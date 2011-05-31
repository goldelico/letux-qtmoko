#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags) :
        QMainWindow(parent),
        ui(new Ui::MainWindow),
        redLed("org.freesmartphone.odeviced", "/org/freesmartphone/Device/LED/gta02_red_aux", QDBusConnection::systemBus(), this),
        blueLed("org.freesmartphone.odeviced", "/org/freesmartphone/Device/LED/gta02_blue_power", QDBusConnection::systemBus(), this),
        orangeLed("org.freesmartphone.odeviced", "/org/freesmartphone/Device/LED/gta02_orange_power", QDBusConnection::systemBus(), this),
        gsmDev("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device", QDBusConnection::systemBus(), this),
        gsmNet("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device", QDBusConnection::systemBus(), this),
        gsmCall("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device", QDBusConnection::systemBus(), this),
        gsmSms("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device", QDBusConnection::systemBus(), this),
        gsmStatusReply(),
        gsmSignalReply(),
        gsmMessageSizeReply()
{
    ui->setupUi(this);

    connect(&gsmCall,
            SIGNAL(CallStatus(int, const QString &, const QVariantMap &)),
            this,
            SLOT(gsmCallStatusChange(int, const QString &, const QVariantMap &)));

    connect(&gsmNet,
            SIGNAL(IncomingUssd(const QString &, const QString &)),
            this,
            SLOT(incomingUssd(const QString &, const QString &)));

    connect(&gsmSms,
            SIGNAL(IncomingTextMessage(const QString &, const QString &, const QString &)),
            this,
            SLOT(incomingTextMessage(const QString &, const QString &, const QString &)));

    connect(&gsmSms,
            SIGNAL(IncomingMessageReport(int, const QString &, const QString &, const QString &)),
            this,
            SLOT(incomingMessageReport(int, const QString &, const QString &, const QString &)));

    QTimer::singleShot(1000, this, SLOT(refresh()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::checkIface(QDBusAbstractInterface *iface)
{
    QString status = (iface->isValid() ? "OK" : "INVALID");
    ui->tbInit->append(status + " " + iface->service() + " " + iface->path());
}

void MainWindow::showVariantMapResult(QDBusPendingReply<QVariantMap> reply, QString caption)
{
    QVariantMap map = reply.value();
    QString str;
    for(int i = 0; i < map.count(); i++)
    {
        QString key = map.keys().at(i);
        str += key + ": " + map.value(key).toString() + "\n";
    }
    QMessageBox::information(this, caption, str);
}

void toggleLed(QFsoDeviceLED *led, int state)
{
    int brightness = (state == Qt::Checked ? 255 : 0);
    led->SetBrightness(brightness);
}

void MainWindow::on_cbRedLed_stateChanged(int state)
{
    toggleLed(&redLed, state);
}

void MainWindow::on_cbBlueLed_stateChanged(int state)
{
    toggleLed(&blueLed, state);
}

void MainWindow::on_cbOrangeLed_stateChanged(int state)
{
    toggleLed(&orangeLed, state);
}

void MainWindow::refresh()
{
    // Init tab
    if(ui->tabInit->isVisible())
    {
        ui->tbInit->clear();
        checkIface(&redLed);
        checkIface(&blueLed);
        checkIface(&orangeLed);
        checkIface(&gsmDev);
        checkIface(&gsmNet);
        checkIface(&gsmCall);
        checkIface(&gsmSms);
    }

    // GSM device & network
    if(ui->tabGsmDev->isVisible())
    {
        // Device status
        if(checkReply(gsmStatusReply, "GSM status", false, false, ui->lGsmStatus))
        {
            ui->lGsmStatus->setText("GSM status: " + gsmStatusReply.value());
        }
        if(gsmStatusReply.isFinished() || gsmStatusReply.isError())
        {
            gsmStatusReply = gsmDev.GetDeviceStatus();
        }
    }
    if(ui->tabGsmNet->isVisible())
    {
        // Signal strength
        if(checkReply(gsmSignalReply, "Signal strength", false, false, ui->lSignalStrength))
        {
            ui->lSignalStrength->setText(QString("Signal strength: %1").arg(gsmSignalReply.value()));
        }
        if(gsmSignalReply.isFinished() || gsmSignalReply.isError())
        {
            gsmSignalReply = gsmNet.GetSignalStrength();
        }
    }

    // SMS
    if(ui->tabGsmSms->isVisible())
    {
        if(checkReply(gsmMessageSizeReply, "Message size", false, false, ui->lSmsSplit))
        {
            ui->lSmsSplit->setText(QString("The message will be splitted in %1 parts").arg(gsmMessageSizeReply.value()));
        }
        if(gsmMessageSizeReply.isFinished() || gsmMessageSizeReply.isError())
        {
            gsmMessageSizeReply = gsmSms.GetSizeForTextMessage(ui->tbSmsContent->toPlainText());
        }
    }

    QTimer::singleShot(1000, this, SLOT(refresh()));
}

void MainWindow::on_bRegister_clicked()
{
    QDBusPendingReply<void> reply = gsmNet.Register();
    checkReply(reply, "Register", true, true);
}

void MainWindow::on_bUnregister_clicked()
{
    QDBusPendingReply<void> reply = gsmNet.Unregister();
    checkReply(reply, "Unregister", true, true);
}

void MainWindow::on_bCall_clicked()
{
    QDBusPendingReply<int> reply = gsmCall.Initiate(ui->tbCallNum->text(), "voice");
    if(checkReply(reply, "Initiate", false, true))
    {
        ui->tbCallId->setText(QString::number(reply.value()));
    }
}

void MainWindow::on_bGsmFeatures_clicked()
{
    QDBusPendingReply<QVariantMap> reply = gsmDev.GetFeatures();
    if(checkReply(reply, "GetFeatures", false, true))
    {
        showVariantMapResult(reply, "Modem features");
    }
}

void MainWindow::on_bListProviders_clicked()
{
    QDBusPendingReply<QFsoNetworkProviderList> reply = gsmNet.ListProviders();
    if(checkReply(reply, "ListProviders", false, true))
    {
        QString str;
//        QFsoNetworkProviderList list = reply.value();
//        for(int i = 0; i < list.count(); i++)
//        {
//            QFsoNetworkProvider provider = list.at(i);
//            str += "shortname=" + provider.shortname +
//                   ", longname=" + provider.longname +
//                   ", mccmnc=" + provider.mccmnc +
//                   ", act=" + provider.act +
//                   ", status=" + provider.status + "\n";
//        }
        QMessageBox::information(this, "Network providers", str);
    }
}

void MainWindow::on_bListCalls_clicked()
{
    QDBusPendingReply<QFsoCallDetailList> reply = gsmCall.ListCalls();
    if(checkReply(reply, "ListCalls", false, true))
    {
        QString str;
        QFsoCallDetailList list = reply.value();
        for(int i = 0; i < list.count(); i++)
        {
            QFsoCallDetail call = list.at(i);
            str += "id=" + QString::number(call.id) +
                   ", status=" + call.status + "\n";

            QVariantMap properties = call.properties;
            for(int j = 0; j < properties.count(); j++)
            {
                QString key = properties.keys().at(j);
                str += ", " + key + "=" + properties.value(key).toString();
            }
        }
        QMessageBox::information(this, "Call list", str);
    }
}

void MainWindow::gsmCallStatusChange(int id, const QString &status, const QVariantMap &properties)
{
    QString str = QString("id=%1, status=%2").arg(id).arg(status);
    for(int i = 0; i < properties.count(); i++)
    {
        QString key = properties.keys().at(i);
        str += ", " + key + "=" + properties.value(key).toString();
    }
    qDebug() << "CallStatusChange" << str;
    ui->lLastCall->setText("Last call: " + str);
    ui->tbCallId->setText(QString::number(id));
}

void MainWindow::incomingUssd(const QString &mode, const QString &message)
{
    QMessageBox::information(this, "Incomming ussd", "mode=" + mode + ", message=" + message);
}

void MainWindow::on_bUssdReq_clicked()
{
    QDBusPendingReply<> reply = gsmNet.SendUssdRequest(ui->tbUssdReq->text());
    checkReply(reply, "SendUssdRequest", true, true);
}

void MainWindow::on_bActivateCall_clicked()
{
    QDBusPendingReply<> reply = gsmCall.Activate(ui->tbCallId->text().toInt());
    checkReply(reply, "Activate", true, true);
}

void MainWindow::on_bActivateConference_clicked()
{
    QDBusPendingReply<> reply = gsmCall.ActivateConference(ui->tbCallId->text().toInt());
    checkReply(reply, "ActivateConference", true, true);
}

void MainWindow::on_bRelease_clicked()
{
    QDBusPendingReply<> reply = gsmCall.Release(ui->tbCallId->text().toInt());
    checkReply(reply, "Release", true, true);
}

void MainWindow::on_bHoldActive_clicked()
{
    QDBusPendingReply<> reply = gsmCall.HoldActive();
    checkReply(reply, "HoldActive", true, true);
}

void MainWindow::on_bJoin_clicked()
{
    QDBusPendingReply<> reply = gsmCall.Join();
    checkReply(reply, "Join", true, true);
}

void MainWindow::on_bReleaseHeld_clicked()
{
    QDBusPendingReply<> reply = gsmCall.ReleaseHeld();
    checkReply(reply, "ReleaseHeld", true, true);
}

void MainWindow::on_bReleaseAll_clicked()
{
    QDBusPendingReply<> reply = gsmCall.ReleaseAll();
    checkReply(reply, "ReleaseAll", true, true);
}

void MainWindow::on_bTransfer_clicked()
{
    QDBusPendingReply<> reply = gsmCall.Transfer(ui->tbTransferNumber->text());
    checkReply(reply, "Transfer", true, true);
}

void MainWindow::incomingTextMessage(const QString &number, const QString &timestamp, const QString &contents)
{
    QMessageBox::information(this, "Incomming SMS",
                             "From: " + number + "\n" +
                             "Time: " + timestamp + "\n" +
                             contents);
}

void MainWindow::incomingMessageReport(int reference, const QString &status, const QString &sender_number, const QString &contents)
{
    QMessageBox::information(this, "SMS status report",
                             "Reference: " + QString::number(reference) + "\n" +
                             "Status: " + status + "\n" +
                             "Sender number: " + sender_number + "\n" +
                             contents);
}

void MainWindow::on_bSend_clicked()
{
    QDBusPendingReply<int, QString> reply = gsmSms.SendTextMessage(ui->tbSmsPhoneNumber->text(),
                                                                   ui->tbSmsContent->toPlainText(),
                                                                   ui->cbReport->checkState() == Qt::Checked);
    if(checkReply(reply, "SendTextMessage", true, true))
    {
        QMessageBox::information(this, "SMS sent", "SMS sent ok, reference=" +
                                 reply.argumentAt(0).toString() +
                                 ", timestamp=" + reply.argumentAt(1).toString());
    }
}

void MainWindow::on_bGetFunctionality_clicked()
{
    QDBusPendingReply<QString, bool, QString> reply = gsmDev.GetFunctionality();
    if(checkReply(reply, "GetFunctionality", false, true))
    {
        QMessageBox::information(this, "GetFunctionality",
                                 "level=" + reply.argumentAt(0).toString() +
                                 ", autoregister=" + reply.argumentAt(1).toString() +
                                 ", pin=" + reply.argumentAt(2).toString());
    }
}

void MainWindow::on_bGetStatus_clicked()
{
    QDBusPendingReply<QVariantMap> reply = gsmNet.GetStatus();
    if(checkReply(reply, "GetStatus", false, true))
    {
        showVariantMapResult(reply, "Network status");
    }
}
