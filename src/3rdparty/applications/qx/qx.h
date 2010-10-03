#ifndef QX_H
#define QX_H

#include <QMainWindow>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include <QMenuBar>
#include <QSettings>

#include <QStringList>
#include "DesktopScanner.h"

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
#include <QFile>
#include <QTcpSocket>
#include <QThread>
#ifdef QTOPIA
#include "qxservice.h"
#include <Qtopia>
#include <QtopiaApplication>
#include <QSoftMenuBar>
#endif
#include "rotate.h"
#include "fakekey.h"
#include "wm.h"
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

    QSettings *favConf;
    QListWidget *lw;
    QAction *favouritesAction;
    QStringList favourites;
    DesktopScanner *scanner;
    QGridLayout* grid;
    //QVBoxLayout* layout;
    QLineEdit *lineEdit;
    QPushButton *bOk;
    //QPushButton *bTango;
    //QPushButton *bScummvm;
    //QPushButton *bQuit;
    QProcess *process;
    QProcess *xprocess;
    Display *dpy;
    FakeKey *fakeKey;
    QTimer *wmTimer;
    RotateHelper *rotHelper;
    QString appName;
    bool rotate;
    bool terminating;
    bool wm;
    bool kbd;
    bool fullscreen;
#if QTOPIA
    QtopiaApplication::PowerConstraint powerConstraint;
#endif

    void stopX();
    void runApp(QString filename, QString applabel, bool rotate);
    void showScreen(QX::Screen scr);

    void FillApps(bool filter);
    void LoadFavourites();
    void BuildMenu();
    int GetClickedId();

    QLabel *lAppname;
    QPushButton *bResume;
    QPushButton *bTerminate;

    QMenu *menu;

public slots:
    void pauseApp();

private slots:
    void okClicked();
    //void tangoClicked();
    //void scummvmClicked();
    void keyPress(QKeyEvent *);
    void keyRelease(QKeyEvent *);
    void processWmEvents();
    void quitClicked();
    void resumeApp();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

    void resumeClicked();
    void terminateClicked();

    void launch_clicked();
    void settings_clicked();
    void info_clicked();
    void favourites_clicked();
    void listClicked();
};

class Sleeper : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};

class QxMainWindow : public QMainWindow
{
public:
    QxMainWindow(QWidget *parent = 0, Qt::WFlags f = 0);
    ~QxMainWindow();
};

#endif // QX_H
