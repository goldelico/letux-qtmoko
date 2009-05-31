#ifndef QMPLAYER_H
#define QMPLAYER_H

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
#include <QBuffer>
#include <QLineEdit>
#ifdef QT_QWS_FICGTA01
#include <QtopiaApplication>
#endif
#ifdef Q_WS_WIN
#include <Windows.h>
#endif

class QMplayer : public QWidget
{
    Q_OBJECT

public:
    QMplayer(QWidget *parent = 0, Qt::WFlags f = 0);
    ~QMplayer();

private:
    enum Screen
    {
        ScreenInit,
        ScreenScan,
        ScreenPlay,
        ScreenFullscreen,
        ScreenStopped,
        ScreenDownload,
        ScreenConnect,
    };

    Screen screen;
    int maxScanLevel;
    bool fbset;
    int delTmpFiles;
    bool abort;
    QVBoxLayout* layout;
    QHBoxLayout* buttonLayout;
    QListWidget* lw;
    QListWidgetItem *scanItem;
    QListWidgetItem *settingsItem;
    QLabel* label;
    QLineEdit *lineEdit;
    QPushButton* bOk;
    QPushButton* bBack;
    QPushButton* bUp;
    QPushButton* bDown;
    QProcess* process;
    QProgressBar *progress;
    QTcpServer *tcpServer;

    void showScreen(QMplayer::Screen scr);
    void scan();
    int scanDir(QString const& path, int level, int maxLevel, int min, int max, bool followSymLinks);
    void settings();
    void play(QStringList const& args);
    void setRes(int xy);
    bool installMplayer();
    bool runServer();
    bool runClient();
    bool download(QString url, QString destPath, QString filename, bool justCheck);

protected:
    void mousePressEvent(QMouseEvent * event);

private slots:
    void okClicked();
    void backClicked();
    void upClicked();
    void downClicked();
    void newConnection();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif // QMPLAYER_H
