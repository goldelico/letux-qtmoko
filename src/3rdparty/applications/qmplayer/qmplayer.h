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
#include <QMainWindow>
#include <QPainter>
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


// This is fullscreen dialog displayed when mplayer application is running. We need
// to avoid any Qtopia drawing when mplayer is running so that it's screen is not
// damaged by Qtopia redraws.
class QMplayerFullscreen : public QWidget
{
    Q_OBJECT

public:
    QMplayerFullscreen();
    void showScreen();

signals:
    void deactivated();
    void pause();
    void volumeUp();
    void volumeDown();

protected:
    bool event(QEvent *);
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void enterFullScreen();

private:
    int downX;
    int lastX;
};

class QMplayer : public QWidget
{
    Q_OBJECT

public:
    QMplayer(QWidget *parent = 0, Qt::WFlags f = 0);
    ~QMplayer();

    enum Screen
    {
        ScreenInit,
        ScreenScan,
        ScreenFullscreen,
        ScreenStopped,
        ScreenDownload,
        ScreenConnect,
        ScreenTube,
        ScreenCmd,
        ScreenEncoding,
        ScreenEncodingInProgress,
    };

private:
    Screen screen;
    int maxScanLevel;
    int delTmpFiles;
    bool abort;
    int useBluetooth;
    QMplayerFullscreen fs;
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
    void play(QStringList & args);
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
    bool isPlaylist(QString fileName);

protected:
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
    void pauseMplayer();
    void volumeUp();
    void volumeDown();
};

class QMplayerMainWindow : public QMainWindow
{
public:
    QMplayerMainWindow(QWidget *parent = 0, Qt::WFlags f = 0);
    ~QMplayerMainWindow();
};

#endif // QMPLAYER_H
