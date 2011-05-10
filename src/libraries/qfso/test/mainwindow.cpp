#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "org.freesmartphone.Device.LED.h"
#include "org.freesmartphone.GSM.Device.h"

MainWindow::MainWindow(QWidget *parent, Qt::WFlags) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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

void toggleLed(QString path, int state, QObject *parent)
{
    OrgFreesmartphoneDeviceLEDInterface led("org.freesmartphone.odeviced",
                                            path,
                                            QDBusConnection::systemBus(), parent);

    int brightness = (state == Qt::Checked ? 255 : 0);
    led.SetBrightness(brightness);
}

void MainWindow::on_cbRedLed_stateChanged(int state)
{
    toggleLed("/org/freesmartphone/Device/LED/gta02_red_aux", state, this);
}

void MainWindow::on_cbBlueLed_stateChanged(int state)
{
    toggleLed("/org/freesmartphone/Device/LED/gta02_blue_power", state, this);
}

void MainWindow::on_cbOrangeLed_stateChanged(int state)
{
    toggleLed("/org/freesmartphone/Device/LED/gta02_orange_power", state, this);
}

void MainWindow::refresh()
{
    // GSM status
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

        OrgFreesmartphoneGSMDeviceInterface gsm("org.freesmartphone.ogsmd",
                                                "/org/freesmartphone/GSM/Device",
                                                QDBusConnection::systemBus(), this);
        gsmStatusReply = gsm.GetDeviceStatus();
    }
    else
    {
        ui->lGsmStatus->setText(ui->lGsmStatus->text() + ".");
    }

    QTimer::singleShot(1000, this, SLOT(refresh()));
}

void MainWindow::on_pushButton_clicked()
{
}
