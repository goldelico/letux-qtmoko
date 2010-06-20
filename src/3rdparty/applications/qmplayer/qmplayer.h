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
#include <QTextCodec>
#include <QLineEdit>
#include <QStringList>
#include <QTimer>
#include <QMenuBar>
#ifdef QTOPIA
#include <QSoftMenuBar>
#include <QtopiaApplication>
#else
#include <QCoreApplication>
#endif
#ifdef Q_WS_WIN
#include <Windows.h>
#endif

#define NUM_MENU_ITEMS 3

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
        ScreenTube,
        ScreenCmd,
        ScreenEncoding,
        ScreenEncodingInProgress,
    };

    Screen screen;
    int maxScanLevel;
    int delTmpFiles;
    bool abort;
    QVBoxLayout* layout;
    QHBoxLayout* buttonLayout;
    QListWidget* lw;
    QListWidgetItem *scanItem;
    QListWidgetItem *sharingItem;
    QListWidgetItem *encodingItem;
    QLabel* label;
    QLineEdit *lineEdit;
    QPushButton* bOk;
    QPushButton* bBack;
    QPushButton* bUp;
    QPushButton* bDown;
    QProcess* process;
    QProgressBar *progress;
    QTcpServer *tcpServer;
    QStringList mpargs;
    QStringList mplist;
#ifdef QTOPIA
    QMenu* softm;
    QAction *rmMpAction, *rmDlAction, *rmFlvAction;
#endif
    bool mpgui;
    bool tube;
    QString ubasedir;
    QString ufname;
    double uload;
    QProcess *upr;
    bool ufinished;

    void showScreen(QMplayer::Screen scr);
    void scan();
    int scanDir(QString const& path, int level, int maxLevel, int min, int max, bool followSymLinks);
    void sharing();
    void play(QStringList const& args);
    void encode(QString filename);
    void setRes(int xy);
    bool installMplayer();
    bool installYoutubeDl();
    bool runServer();
    bool runClient();
    bool download(QString url, QString destPath, QString filename, bool justCheck);
    bool startMencoder(QString srcFile, QString dstFile);
    void console(QString s);
    bool youtubeDl();
    bool runCmd(QString cmd, int maxp=0);
    void playerStopped();
    void setDlText();
    QString getEncFilename(QString srcFile, QString dstIdentifier);

protected:
    void mousePressEvent(QMouseEvent * event);
    void closeEvent(QCloseEvent *event);

private slots:
    void okClicked();
    void backClicked();
    void upClicked();
    void downClicked();
    void newConnection();
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void removeMplayer();
    void removeYoutubeDl();
    void removeFlv();
    void sTimerEvent();
    void uReadyRead();
    void mencoderReadyRead();
    void uFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif // QMPLAYER_H
