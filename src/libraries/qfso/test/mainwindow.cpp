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
        gsmCall("org.freesmartphone.ogsmd", "/org/freesmartphone/GSM/Device", QDBusConnection::systemBus(), this)
{
    ui->setupUi(this);

    OrgFreesmartphoneGSMNetworkProvider::registerMetaType();
    OrgFreesmartphoneGSMNetworkProviderList::registerMetaType();
    OrgFreesmartphoneGSMCallDetail::registerMetaType();
    OrgFreesmartphoneGSMCallDetailList::registerMetaType();

    connect(&gsmCall,
            SIGNAL(CallStatus(int, const QString &, const QVariantMap &)),
            this,
            SLOT(gsmCallStatusChange(int, const QString &, const QVariantMap &)));

    connect(&gsmNet,
            SIGNAL(IncomingUssd(const QString &, const QString &)),
            this,
            SLOT(incomingUssd(const QString &, const QString &)));

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

void toggleLed(OrgFreesmartphoneDeviceLEDInterface *led, int state)
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
    }

    // GSM
    if(ui->tabGsm->isVisible())
    {
        // Device status
        if(checkReply(gsmStatusReply, "GSM status", false, false, ui->lGsmStatus))
        {
            ui->lGsmStatus->setText("GSM status: " + gsmStatusReply.value());
        }
        if(gsmStatusReply.isFinished())
        {
            gsmStatusReply = gsmDev.GetDeviceStatus();
        }

        // Signal strength
        if(checkReply(gsmSignalReply, "Signal strength", false, false, ui->lSignalStrength))
        {
            ui->lSignalStrength->setText(QString("Signal strength: %1").arg(gsmSignalReply.value()));
        }
        if(gsmSignalReply.isFinished())
        {
            gsmSignalReply = gsmNet.GetSignalStrength();
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
        QVariantMap map = reply.value();
        QString str;
        for(int i = 0; i < map.count(); i++)
        {
            QString key = map.keys().at(i);
            str += key + ": " + map.value(key).toString() + "\n";
        }
        QMessageBox::information(this, "Modem features", str);
    }
}

void MainWindow::on_bListProviders_clicked()
{
    QDBusPendingReply<OrgFreesmartphoneGSMNetworkProviderList> reply = gsmNet.ListProviders();
    if(checkReply(reply, "ListProviders", false, true))
    {
        QString str;
        OrgFreesmartphoneGSMNetworkProviderList list = reply.value();
        for(int i = 0; i < list.count(); i++)
        {
            OrgFreesmartphoneGSMNetworkProvider provider = list.at(i);
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
    QDBusPendingReply<OrgFreesmartphoneGSMCallDetailList> reply = gsmCall.ListCalls();
    if(checkReply(reply, "ListCalls", false, true))
    {
        QString str;
        OrgFreesmartphoneGSMCallDetailList list = reply.value();
        for(int i = 0; i < list.count(); i++)
        {
            OrgFreesmartphoneGSMCallDetail call = list.at(i);
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
