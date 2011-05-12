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
        if(gsmStatusReply.isFinished())
        {
            if(gsmStatusReply.isValid())
            {
                ui->lGsmStatus->setText("GSM status: " + gsmStatusReply.value());
            }
            else
            {
                ui->lGsmStatus->setText("GSM status: error " + gsmStatusReply.error().message());
            }
            gsmStatusReply = gsmDev.GetDeviceStatus();
        }
        else
        {
            ui->lGsmStatus->setText(ui->lGsmStatus->text() + ".");
        }

        // Signal strength
        if(gsmSignalReply.isFinished())
        {
            if(gsmSignalReply.isValid())
            {
                ui->lSignalStrength->setText(QString("Signal strength: %1").arg(gsmSignalReply.value()));
            }
            else
            {
                ui->lSignalStrength->setText("Signal strength: " + gsmSignalReply.error().message());
            }
            gsmSignalReply = gsmNet.GetSignalStrength();
        }
        else
        {
            ui->lSignalStrength->setText(ui->lSignalStrength->text() + ".");
        }
    }  


    QTimer::singleShot(1000, this, SLOT(refresh()));
}

void MainWindow::on_bRegister_clicked()
{
    QDBusPendingReply<void> reply = gsmNet.Register();
    reply.waitForFinished();
    if(reply.isValid())
    {
        QMessageBox::information(this, "Register", "Register ok");
    }
    else
    {
        QMessageBox::critical(this, "Register", "Register failed: " + reply.error().message());
    }
}

void MainWindow::on_bUnregister_clicked()
{
    QDBusPendingReply<void> reply = gsmNet.Unregister();
    reply.waitForFinished();
    if(reply.isValid())
    {
        QMessageBox::information(this, "Unregister", "Unregister ok");
    }
    else
    {
        QMessageBox::critical(this, "Unregister", "Unregister failed: " + reply.error().message());
    }
}
