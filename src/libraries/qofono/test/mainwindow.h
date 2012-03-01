#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDebug>
#include <QMainWindow>
#include <QtDBus>
#include <QTimer>
#include <QMessageBox>
#include <QLabel>

#include "ofonoutil.h"

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

    void checkIface(QDBusAbstractInterface *iface);
    void showVariantMap(QVariantMap map, QString caption);
    void showVariantMapResult(QOFonoDBusPendingReply<QVariantMap> reply, QString caption);

    template <class T, class T2, class T3>
            bool checkReply2(QOFonoDBusPendingReply<T,T2,T3> & reply,
                             bool okBox,
                             bool waitForFinished,
                             QLabel * label = NULL);

    template <class T, class T2, class T3>
            void watchCallWatcher(QDBusPendingReply<T,T2,T3> & reply,
                                  QDBusPendingCallWatcher *watcher,
                                  const char * finishedMethod);

Q_SIGNALS:
    void finished(QDBusPendingReply<> *reply);

private slots:
    void on_bRetrieveTextMessages_clicked();
    void on_bDeactivateContext_clicked();
    void on_tbActivateContext_clicked();
    void on_bSetCredentials_clicked();
    void on_bSimInfo_clicked();
    void on_bQueryMessages_clicked();
    void on_tbSmsContent_textChanged();
    void on_bGetStatus_clicked();
    void on_bGetFunctionality_clicked();
    void on_bSend_clicked();
    void on_bTransfer_clicked();
    void on_bReleaseAll_clicked();
    void on_bReleaseHeld_clicked();
    void on_bJoin_clicked();
    void on_bHoldActive_clicked();
    void on_bRelease_clicked();
    void on_bActivateConference_clicked();
    void on_bActivateCall_clicked();
    void on_bUssdReq_clicked();
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
    void incomingTextMessage(const QString &number, const QString &timestamp, const QString &contents);
    void incomingMessageReport(int reference, const QString &status, const QString &sender_number, const QString &contents);
    void gsmMessageSizeFinished(QOFonoDBusPendingCall & call);
};

template <class T, class T2, class T3>
        bool MainWindow::checkReply2(QOFonoDBusPendingReply<T, T2, T3> & reply,
                                     bool okBox,
                                     bool waitForFinished,
                                     QLabel * label)
{
    if(waitForFinished)
    {
        reply.waitForFinished();
    }

    //    qWarning() << "reply.isFinished()=" << reply.isFinished() <<
    //            ", reply.isValid()=" << reply.isValid() <<
    //            ", reply.isError()=" << reply.isError() <<
    //            ", fn=" << fn;

    if(reply.isError())
    {
        QString err = reply.error().message();
        qWarning() << "Error in " << reply.methodCall << err;
        if(label != NULL)
        {
            label->setText(reply.method + ": " + err);
        }
        else
        {
            QMessageBox::critical(this, reply.method, reply.method + " failed: " + err);
        }
        return false;
    }
    if(reply.isFinished() && reply.isValid())
    {
        if(okBox)
        {
            QMessageBox::information(this, reply.method, reply.method + " ok");
        }
        return true;
    }
    if(label != NULL)
    {
        label->setText(label->text() + ".");
    }
    return false;
}

template <class T, class T2, class T3>
        void MainWindow::watchCallWatcher(QDBusPendingReply<T,T2,T3> & reply,
                                          QDBusPendingCallWatcher *watcher,
                                          const char * finishedMethod)
{
    watcher->~QDBusPendingCallWatcher();
    watcher = new (watcher)QDBusPendingCallWatcher(reply, this);
    QObject::connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher*)),
                     this, finishedMethod);
}

#endif // MAINWINDOW_H
