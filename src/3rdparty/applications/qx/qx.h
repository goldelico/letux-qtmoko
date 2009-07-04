#ifndef QX_H
#define QX_H

#include <QWidget>
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

    QVBoxLayout* layout;
    QLineEdit *lineEdit;
    QLabel *label;
    QPushButton *bRun;
    QPushButton *bTango;
    QPushButton *bQuit;
    QProcess *process;
    QTimer *killTimer;

    void showScreen(QX::Screen scr);
    void runApp(QString filename);

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    bool event(QEvent *);
    void enterFullScreen();
    void exitFullScreen();

private slots:
    void runClicked();
    void tangoClicked();
    void quitClicked();
    void killTimerElapsed();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif // QX_H
