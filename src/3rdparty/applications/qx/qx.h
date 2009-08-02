#ifndef QX_H
#define QX_H

#include "apprunningscreen.h"
#include <QWidget>
#include <QPixmap>
#include <QEvent>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QProcess>
#include <QApplication>
#include <QMouseEvent>
#include <QLineEdit>
#include <QMessageBox>
#include <QTimer>
#include <QDesktopWidget>
#include <QFile>
#include <QTcpSocket>
#include <QThread>
#ifdef QT_QWS_FICGTA01
#include "qxservice.h"
#include <QtopiaApplication>
#include <QDeviceButtonManager>
#endif
#include <unistd.h>
#include <fcntl.h>
#include <linux/vt.h>
#include <sys/ioctl.h>
#include <X11/Xlib.h>

class QX : public QWidget
{
    Q_OBJECT

public:
    QX(QWidget *parent = 0, Qt::WFlags f = 0);
    ~QX();

private:
    enum Screen
    {
        ScreenMain,
        ScreenPaused,
        ScreenFullscreen,   // full screen modes go down here
        ScreenStarting,
        ScreenRunning,
    };

    Screen screen;
    AppRunningScreen *appRunScr;

    QVBoxLayout* layout;
    QLineEdit *lineEdit;
    QPushButton *bOk;
    QPushButton *bTango;
    QPushButton *bScummvm;
    QPushButton *bQuit;
    QProcess *process;
    QProcess *xprocess;
    QString appName;
    bool rotate;
    bool terminating;
#if QT_QWS_FICGTA01
    QtopiaApplication::PowerConstraint powerConstraint;
    QtopiaServiceRequest origSrq;
#endif

    void stopX();
    void runApp(QString filename, bool rotate);
    void showScreen(QX::Screen scr);

public slots:
    void pauseApp();

private slots:
    void okClicked();
    void tangoClicked();
    void scummvmClicked();
    void quitClicked();
    void resumeApp();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

class Sleeper : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};


#endif // QX_H
