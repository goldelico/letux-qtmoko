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
        ScreenMplayerInstall,
        ScreenPlay,
        ScreenFullscreen,
        ScreenStopped,
    };

    Screen screen;
    int maxScanLevel;
    QVBoxLayout* layout;
    QHBoxLayout* buttonLayout;
    QListWidget* lw;
    QListWidgetItem *scanItem;
    QListWidgetItem *settingsItem;
    QLabel* label;
    QPushButton* bOk;
    QPushButton* bBack;
    QPushButton* bUp;
    QPushButton* bDown;
    QProcess* process;
    QProgressBar *progress;

    void showScreen(QMplayer::Screen scr);
    void scan();
    void scanDir(QString const& path, int level, int maxLevel, int min, int max);
    void settings();
    void play(QStringList const& args);
    void setRes(int xy);
    bool installMplayer();
    bool runProcess(QString const& info, QProcess *p, QString const& program, QStringList const& args);

protected:
    void mousePressEvent(QMouseEvent * event);

private slots:
    void okClicked();
    void backClicked();
    void upClicked();
    void downClicked();
};

#endif // QMPLAYER_H
