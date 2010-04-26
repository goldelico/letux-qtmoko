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
        ScreenSysfs,
        ScreenDisplay,
    };

    Screen screen;
    QVBoxLayout* layout;
    QHBoxLayout* buttonLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QPushButton* bQvga;
    QPushButton* bBack;
    QPushButton* bNext;

    void showScreen(NeoControl::Screen scr);

private slots:
    void qvgaClicked();
    void backClicked();
    void nextClicked();
    void updateSysfs();
};

#endif // NEOCONTROL_H
