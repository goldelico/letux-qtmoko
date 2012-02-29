#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ofonoutil.h"

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
        gsmSim("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device", QDBusConnection::systemBus(), this),
        gsmPdp("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device", QDBusConnection::systemBus(), this),
        pimMsg("org.freesmartphone.opimd", "/org/freesmartphone/PIM/Messages", QDBusConnection::systemBus(), this),
        ofonoUsage("org.freesmartphone.ousaged", "/org/freesmartphone/Usage", QDBusConnection::systemBus(), this),
        gsmStatusReply(),
        gsmSignalReply(),
        gsmMessageSizeWatcher(QDBusPendingReply<uint>(), this)
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

void MainWindow::showVariantMap(QVariantMap map, QString caption)
{
    QString str;
    for(int i = 0; i < map.count(); i++)
    {
        QString key = map.keys().at(i);
        str += key + ": " + map.value(key).toString() + "\n";
    }
    QMessageBox::information(this, caption, str);
}

void MainWindow::showVariantMapResult(QOFonoDBusPendingReply<QVariantMap> reply, QString caption)
{
    showVariantMap(reply.value(), caption);
}

void toggleLed(QOFonoDeviceLED *led, int state)
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
        checkIface(&pimMsg);
    }

    // GSM device & network
    if(ui->tabGsmDev->isVisible())
    {
        // Device status
        if(checkReply2(gsmStatusReply, false, false, ui->lGsmStatus))
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
        if(checkReply2(gsmSignalReply, false, false, ui->lSignalStrength))
        {
            ui->lSignalStrength->setText(QString("Signal strength: %1").arg(gsmSignalReply.value()));
        }
        if(gsmSignalReply.isFinished() || gsmSignalReply.isError())
        {
            gsmSignalReply = gsmNet.GetSignalStrength();
        }
    }

    QTimer::singleShot(1000, this, SLOT(refresh()));
}

void MainWindow::on_bRegister_clicked()
{
    QOFonoDBusPendingReply<void> reply = gsmNet.Register();
    checkReply2(reply, true, true);
}

void MainWindow::on_bUnregister_clicked()
{
    QOFonoDBusPendingReply<void> reply = gsmNet.Unregister();
    checkReply2(reply, true, true);
}

void MainWindow::on_bCall_clicked()
{
    QOFonoDBusPendingReply<int> reply = gsmCall.Initiate(ui->tbCallNum->text(), "voice");
    if(checkReply2(reply, false, true))
    {
        ui->tbCallId->setText(QString::number(reply.value()));
    }
}

void MainWindow::on_bGsmFeatures_clicked()
{
    QOFonoDBusPendingReply<QVariantMap> reply = gsmDev.GetFeatures();
    if(checkReply2(reply, false, true))
    {
        showVariantMapResult(reply, "Modem features");
    }
}

void MainWindow::on_bListProviders_clicked()
{
    QOFonoDBusPendingReply<QOFonoNetworkProviderList> reply = gsmNet.ListProviders();
    if(checkReply2(reply, false, true))
    {
        QString str;
        QOFonoNetworkProviderList list = reply.value();
        for(int i = 0; i < list.count(); i++)
        {
            QOFonoNetworkProvider provider = list.at(i);
            str += "shortname=" + provider.shortname +
                   ", longname=" + provider.longname +
                   ", mccmnc=" + provider.mccmnc +
                   ", act=" + provider.act +
                   ", status=" + provider.status + "\n";
        }
        QMessageBox::information(this, "Network providers", str);
    }
}

void MainWindow::on_bListCalls_clicked()
{
    QOFonoDBusPendingReply<QOFonoCallDetailList> reply = gsmCall.ListCalls();
    if(checkReply2(reply, false, true))
    {
        QString str;
        QOFonoCallDetailList list = reply.value();
        for(int i = 0; i < list.count(); i++)
        {
            QOFonoCallDetail call = list.at(i);
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
    QOFonoDBusPendingReply<> reply = gsmNet.SendUssdRequest(ui->tbUssdReq->text());
    checkReply2(reply, true, true);
}

void MainWindow::on_bActivateCall_clicked()
{
    QOFonoDBusPendingReply<> reply = gsmCall.Activate(ui->tbCallId->text().toInt());
    checkReply2(reply, true, true);
}

void MainWindow::on_bActivateConference_clicked()
{
    QOFonoDBusPendingReply<> reply = gsmCall.ActivateConference(ui->tbCallId->text().toInt());
    checkReply2(reply, true, true);
}

void MainWindow::on_bRelease_clicked()
{
    QOFonoDBusPendingReply<> reply = gsmCall.Release(ui->tbCallId->text().toInt());
    checkReply2(reply, true, true);
}

void MainWindow::on_bHoldActive_clicked()
{
    QOFonoDBusPendingReply<> reply = gsmCall.HoldActive();
    checkReply2(reply, true, true);
}

void MainWindow::on_bJoin_clicked()
{
    QOFonoDBusPendingReply<> reply = gsmCall.Join();
    checkReply2(reply, true, true);
}

void MainWindow::on_bReleaseHeld_clicked()
{
    QOFonoDBusPendingReply<> reply = gsmCall.ReleaseHeld();
    checkReply2(reply, true, true);
}

void MainWindow::on_bReleaseAll_clicked()
{
    QOFonoDBusPendingReply<> reply = gsmCall.ReleaseAll();
    checkReply2(reply, true, true);
}

void MainWindow::on_bTransfer_clicked()
{
    QOFonoDBusPendingReply<> reply = gsmCall.Transfer(ui->tbTransferNumber->text());
    checkReply2(reply, true, true);
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
    QOFonoDBusPendingReply<int, QString> reply = gsmSms.SendTextMessage(ui->tbSmsPhoneNumber->text(),
                                                                   ui->tbSmsContent->toPlainText(),
                                                                   ui->cbReport->checkState() == Qt::Checked);
    if(checkReply2(reply, true, true))
    {
        QMessageBox::information(this, "SMS sent", "SMS sent ok, reference=" +
                                 reply.argumentAt(0).toString() +
                                 ", timestamp=" + reply.argumentAt(1).toString());
    }
}

void MainWindow::on_bGetFunctionality_clicked()
{
    QOFonoDBusPendingReply<QString, bool, QString> reply = gsmDev.GetFunctionality();
    if(checkReply2(reply, false, true))
    {
        QMessageBox::information(this, "GetFunctionality",
                                 "level=" + reply.argumentAt(0).toString() +
                                 ", autoregister=" + reply.argumentAt(1).toString() +
                                 ", pin=" + reply.argumentAt(2).toString());
    }
}

void MainWindow::on_bGetStatus_clicked()
{
    QOFonoDBusPendingReply<QVariantMap> reply = gsmNet.GetStatus();
    checkReply(reply);
    if(checkReply2(reply, false, true))
    {
        showVariantMapResult(reply, "Network status");
    }
}

void MainWindow::on_tbSmsContent_textChanged()
{
    QOFonoDBusPendingCall call = gsmSms.GetSizeForTextMessage(ui->tbSmsContent->toPlainText());
    watchOFonoCall(call, this, SLOT(gsmMessageSizeFinished(QOFonoDBusPendingCall &)));
}

void MainWindow::gsmMessageSizeFinished(QOFonoDBusPendingCall & call)
{
    QOFonoDBusPendingReply<uint> reply = call;
    if(checkReply2(reply, false, false))
    {
        ui->lSmsSplit->setText(QString("The message will be splitted in %1 parts").arg(reply.value()));
    }
}

void MainWindow::on_bQueryMessages_clicked()
{
    QOFonoDBusPendingReply<QString> reply = pimMsg.Query(QVariantMap());
    if(!checkReply2(reply, false, true))
    {
        return;
    }
    QString path = reply.value();
    QOFonoPIMMessageQuery q("org.freesmartphone.opimd", path, QDBusConnection::systemBus(), this);

    QOFonoDBusPendingReply<QOFonoVariantMapList> reply2 = q.GetMultipleResults(-1);
    if(!checkReply2(reply2, false, true))
    {
        return;
    }
    QOFonoVariantMapList list = reply2.value();
    for(int i = 0; i < list.count(); i++)
    {
        QVariantMap msg = list.at(i);
        showVariantMap(msg, "Message " + QString::number(i));
    }

//    QOFonoDBusPendingReply<QOFonoSIMEntryList> reply = gsmSim.RetrievePhonebook("contacts", 0, 65535);


//    QOFonoDBusPendingReply<int> reply2 = q.GetResultCount();
//    if(!checkReply2(reply2, false, true))
//    {
//        return;
//    }
//    int count = reply2.value();
//    QMessageBox::information(this, "Message count", QString::number(count));
}

void MainWindow::on_bSimInfo_clicked()
{
    QOFonoDBusPendingReply<QVariantMap> reply = gsmSim.GetSimInfo();
    if(checkReply2(reply, false, true))
    {
        showVariantMapResult(reply, "Sim info");
    }
}

void MainWindow::on_bSetCredentials_clicked()
{
    QOFonoDBusPendingReply<> reply = gsmPdp.SetCredentials(ui->tbApn->text(), ui->tbUsername->text(), ui->tbPassword->text());
    checkReply2(reply, true, true);
}

void MainWindow::on_tbActivateContext_clicked()
{
    QOFonoDBusPendingReply<> reply = gsmPdp.ActivateContext();
    checkReply2(reply, true, true);
}

void MainWindow::on_bDeactivateContext_clicked()
{
    QOFonoDBusPendingReply<> reply = gsmPdp.DeactivateContext();
    checkReply2(reply, true, true);
}

void MainWindow::on_bRetrieveTextMessages_clicked()
{
    QOFonoDBusPendingReply<QOFonoSIMMessageList> reply = gsmSms.RetrieveTextMessages();
    if(checkReply2(reply, false, true))
    {
        QOFonoSIMMessageList list = reply.value();
        QString str;
        for(int i = 0; i < list.count(); i++)
        {
            QOFonoSIMMessage msg = list.at(i);
            str += msg.number + " " + msg.contents + "\n";
        }
        QMessageBox::information(this, "SMS messages", str);
    }
}
