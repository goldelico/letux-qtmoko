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
        callId(-1)
{
    ui->setupUi(this);

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
        callId = reply.value();
    }
}
