#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QtDBus>
#include <QTimer>
#include <QMessageBox>
#include <QLabel>

#include "org.freesmartphone.Device.LED.h"
#include "org.freesmartphone.GSM.Device.h"
#include "org.freesmartphone.GSM.Network.h"
#include "org.freesmartphone.GSM.Call.h"

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
    OrgFreesmartphoneGSMDeviceInterface gsmDev;
    OrgFreesmartphoneGSMNetworkInterface gsmNet;
    OrgFreesmartphoneGSMCallInterface gsmCall;

    QDBusPendingReply<QString> gsmStatusReply;
    QDBusPendingReply<int> gsmSignalReply;
    void checkIface(QDBusAbstractInterface *iface);
    template <class T>
            bool checkReply(QDBusPendingReply<T> & reply,
                            const QString & fn,
                            bool okBox,
                            bool waitForFinished,
                            QLabel * label = NULL);

private slots:
    void on_bTransfer_clicked();
    void on_bReleaseAll_clicked();
    void on_bReleaseHeld_clicked();
    void on_bJoin_clicked();
    void on_bHoldActive_clicked();
    void on_bRelease_clicked();
    void on_bActivateConference_clicked();
    void on_bActivateCall_clicked();
    void on_bUssdReq_clicked();
    void on_bListCalls_clicked();
    void on_bListProviders_clicked();
    void on_bGsmFeatures_clicked();
    void on_bCall_clicked();
    void on_bUnregister_clicked();
    void on_bRegister_clicked();
    void on_cbOrangeLed_stateChanged(int );
    void on_cbBlueLed_stateChanged(int );
    void on_cbRedLed_stateChanged(int );
    void refresh();
    void gsmCallStatusChange(int id, const QString &status, const QVariantMap &properties);
    void incomingUssd(const QString &mode, const QString &message);
};

template <class T>
        bool MainWindow::checkReply(QDBusPendingReply<T> & reply,
                                    const QString & fn,
                                    bool okBox,
                                    bool waitForFinished,
                                    QLabel * label)
{
    if(waitForFinished)
    {
        reply.waitForFinished();
    }
    if(reply.isFinished())
    {
        if(reply.isValid())
        {
            if(okBox)
            {
                QMessageBox::information(this, fn, fn + " ok");
            }
            return true;
        }
        else
        {
            QString err = reply.error().message();
            qWarning() << "Error in " << fn << err;
            if(label != NULL)
            {
                label->setText(fn + ": " + err);
            }
            else
            {
                QMessageBox::critical(this, fn, fn + " failed: " + err);
            }
            return false;
        }
    }
    else
    {
        if(label != NULL)
        {
            label->setText(label->text() + ".");
        }
        return false;
    }
}

#endif // MAINWINDOW_H
