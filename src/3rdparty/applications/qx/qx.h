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
#ifdef QT_QWS_FICGTA01
#include <QtopiaApplication>
#endif

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
    QPushButton *bQuit;
    QProcess *process;
    QString appName;

    void runApp(QString filename);
    void showScreen(QX::Screen scr);

private slots:
    void okClicked();
    void tangoClicked();
    void quitClicked();
    void pauseApp();
    void resumeApp();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif // QX_H
