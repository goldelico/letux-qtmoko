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
#include <QTimer>
#include <QDesktopWidget>
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
    QVBoxLayout* layout;
    QLineEdit *lineEdit;
    QLabel *label;
    QPushButton *bRun;
    QPushButton *bQuit;
    QProcess *process;
    QTimer *killTimer;
    bool is_fullscreen;

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    bool event(QEvent *);
    void enterFullScreen();
    void exitFullScreen();

private slots:
    void runClicked();
    void quitClicked();
    void killTimerElapsed();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif // QX_H
