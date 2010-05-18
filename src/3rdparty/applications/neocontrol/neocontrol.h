#ifndef NEOCONTROL_H
#define NEOCONTROL_H

#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QProcess>
#include <QDir>
#include <QProgressDialog>
#include <QErrorMessage>
#include <QProgressBar>
#include <QApplication>
#include <QMouseEvent>
#include <QMessageBox>
#include <QHttp>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextCodec>
#include <QLineEdit>
#include <QTimer>
#include <QSettings>
#include <QCheckBox>
#ifdef QTOPIA
#include <QtopiaApplication>
#endif
#ifdef Q_WS_WIN
#include <Windows.h>
#endif

class NeoControl : public QWidget
{
    Q_OBJECT

public:
    NeoControl(QWidget *parent = 0, Qt::WFlags f = 0);
    ~NeoControl();

private:
    enum Screen
    {
        ScreenInit,
        ScreenModem,
        ScreenSysfs,
        ScreenDisplay,
    };

    Screen screen;
    bool updatingModem;
    QVBoxLayout *layout;
    QHBoxLayout *buttonLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton *bQvga;
    QPushButton *bBack;
    QPushButton *bNext;
    QCheckBox *chkDeepSleep;
    QCheckBox *chkMux;

    void showScreen(NeoControl::Screen scr);

private slots:
    void qvgaClicked();
    void backClicked();
    void nextClicked();
    void updateModem();
    void updateSysfs();
    void deepSleepStateChanged(int);
    void muxStateChanged(int);
};

#endif // NEOCONTROL_H
