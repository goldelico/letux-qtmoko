#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDBus>
#include <QTimer>

#include "org.freesmartphone.Device.LED.h"
#include "org.freesmartphone.GSM.Device.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0, Qt::WFlags f = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::MainWindow *ui;
    OrgFreesmartphoneDeviceLEDInterface redLed;
    OrgFreesmartphoneDeviceLEDInterface blueLed;
    OrgFreesmartphoneDeviceLEDInterface orangeLed;
    OrgFreesmartphoneGSMDeviceInterface gsm;
    QDBusPendingReply<QString> gsmStatusReply;
    void checkIface(QDBusAbstractInterface *iface);

private slots:
    void on_pushButton_clicked();
    void on_cbOrangeLed_stateChanged(int );
    void on_cbBlueLed_stateChanged(int );
    void on_cbRedLed_stateChanged(int );
    void refresh();
};

#endif // MAINWINDOW_H
