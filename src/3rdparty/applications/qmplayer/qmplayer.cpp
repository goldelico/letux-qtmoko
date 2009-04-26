#include "qmplayer.h"

QMplayer::QMplayer(QWidget *parent, Qt::WFlags f)
    : QWidget(parent)
{
#ifdef QT_QWS_FICGTA01
    this->setWindowState(Qt::WindowMaximized);
#endif

    lw = new QListWidget(this);

    scanItem = new QListWidgetItem(tr("Scan"), lw);
    settingsItem = new QListWidgetItem(tr("Settings"), lw);

    bOk = new QPushButton(">", this);
    connect(bOk, SIGNAL(clicked()), this, SLOT(okClicked()));

    bBack = new QPushButton("Back", this);
    connect(bBack, SIGNAL(clicked()), this, SLOT(backClicked()));

    bVolUp = new QPushButton("Volume up", this);
    connect(bVolUp, SIGNAL(clicked()), this, SLOT(volUpClicked()));

    bVolDown = new QPushButton("Volume down", this);
    connect(bVolDown, SIGNAL(clicked()), this, SLOT(volDownClicked()));

    label = new QLabel(this);

    progress = new QProgressBar(this);

    layout = new QVBoxLayout(this);
    layout->addWidget(lw);
    layout->addWidget(bOk);
    layout->addWidget(bBack);
    layout->addWidget(bVolUp);
    layout->addWidget(bVolDown);
    layout->addWidget(label);
    layout->addWidget(progress);

    showScreen(QMplayer::ScreenInit);
}

QMplayer::~QMplayer()
{

}

void QMplayer::mousePressEvent(QMouseEvent * event)
{
    if(screen != QMplayer::ScreenPlay && screen != QMplayer::ScreenStopped)
    {
        return;
    }
    if(process == NULL)
    {
        return;
    }
    QProcess::ProcessState state = process->state();
    if(state == QProcess::NotRunning)
    {
        setRes(640480);
        showScreen(QMplayer::ScreenInit);
        delete(process);
        process = NULL;
        return;
    }
    if(state != QProcess::Running)
    {
        return;
    }

    process->write(" ");
    process->waitForBytesWritten();
    if(screen == QMplayer::ScreenPlay)
    {
        setRes(640480);
        showNormal();
        showScreen(QMplayer::ScreenStopped);
    }
    else if(screen == QMplayer::ScreenStopped)
    {
        showScreen(QMplayer::ScreenPlay);
    }
}

void QMplayer::okClicked()
{
    if(screen == QMplayer::ScreenInit)
    {
        QListWidgetItem *sel = lw->currentItem();

        if(sel == NULL)
        {
            return;
        }
        if(sel == scanItem)
        {
            scan();
            return;
        }
        if(sel == settingsItem)
        {
            settings();
            return;
        }
        play(sel->text());
    }
    else if(screen == QMplayer::ScreenStopped)
    {
        if(process != NULL && (process->state() == QProcess::Running))
        {
            process->write(" ");
        }
        showScreen(QMplayer::ScreenPlay);
    }
}

void QMplayer::backClicked()
{
    if(screen == QMplayer::ScreenInit)
    {
        close();
    }
    else if(screen == QMplayer::ScreenStopped)
    {
        process->write("q");
        process->waitForFinished(4000);
        delete(process);
        showScreen(QMplayer::ScreenInit);
    }
}

void QMplayer::volUpClicked()
{
    if(process != NULL && (process->state() == QProcess::Running))
    {
        if(screen == QMplayer::ScreenStopped)
        {
            process->write("0 ");
        }
        else
        {
            process->write("0");
        }
    }
}

void QMplayer::volDownClicked()
{
    if(process != NULL && (process->state() == QProcess::Running))
    {
        if(screen == QMplayer::ScreenStopped)
        {
            process->write("9 ");
        }
        else
        {
            process->write("9");
        }
    }
}

void QMplayer::showScreen(QMplayer::Screen scr)
{
    lw->setVisible(scr == QMplayer::ScreenInit);
    bOk->setVisible(scr == QMplayer::ScreenInit || scr == QMplayer::ScreenStopped);
    bBack->setVisible(scr == QMplayer::ScreenInit || scr == QMplayer::ScreenStopped);
    bVolUp->setVisible(scr == QMplayer::ScreenPlay || scr == QMplayer::ScreenStopped);
    bVolDown->setVisible(scr == QMplayer::ScreenPlay || scr == QMplayer::ScreenStopped);
    label->setVisible(scr == QMplayer::ScreenScan);
    progress->setVisible(scr == QMplayer::ScreenScan);

    if(scr == QMplayer::ScreenInit)
    {
        bBack->setText(tr("Quit"));
    }
    else if(scr == QMplayer::ScreenPlay)
    {
        bBack->setText(tr("Back"));
        setRes(320240);
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        setWindowState(Qt::WindowFullScreen);
        raise();
    }

    this->screen = scr;
}

void QMplayer::scan()
{
    showScreen(QMplayer::ScreenScan);

    while(lw->count() > 2)
    {
        delete(lw->takeItem(2));
    }

    progress->setMinimum(0);
    progress->setMaximum(0x7fffffff);
#ifdef QT_QWS_FICGTA01
    scanDir("/mnt", 0, 0, 0x2fffffff);
    scanDir("/media", 0, 0, 0x4fffffff);
    scanDir("/home", 0, 0, 0x7fffffff);
    scanDir("/root", 0, 0, 0x7fffffff);
#else
    scanDir("/home/radek/Desktop", 0, 0, 0x7fffffff);
#endif
    showScreen(QMplayer::ScreenInit);
}

void QMplayer::scanDir(QString const& path, int level, int min, int max)
{
    QDir dir(path);
    QFileInfoList list = dir.entryInfoList(QDir::AllEntries, QDir::Name);

    for(int i = 0; i < list.count(); i++)
    {
        QFileInfo fi = list.at(i);
        if(!fi.isFile())
        {
            continue;
        }
        QString absPath = fi.absoluteFilePath();
        if(absPath.endsWith(".mp3", Qt::CaseInsensitive)
            || absPath.endsWith(".ogg", Qt::CaseInsensitive)
            || absPath.endsWith(".ogv", Qt::CaseInsensitive)
            || absPath.endsWith(".avi", Qt::CaseInsensitive))
        {
            lw->addItem(absPath);
        }
    }

    for(int i = 0; i < list.count(); i++)
    {
        QFileInfo fi = list.at(i);
        if(fi.isFile() || fi.isSymLink() || fi.fileName() == "." || fi.fileName() == "..")
        {
            continue;
        }

        int unit = (max - min) / list.count();
        int value = min + i * unit;
        if(progress->value() == value)
        {
            value++;
        }
        progress->setValue(value);
        label->setText(fi.fileName());
        label->repaint();
        scanDir(fi.filePath(), level + 1, value, value + unit);
    }
}

void QMplayer::settings()
{
    // Full screen test
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    setWindowState(Qt::WindowFullScreen);
    raise();
}

void QMplayer::play(QString const& filename)
{
    showScreen(QMplayer::ScreenPlay);

    QStringList args;
#ifdef QT_QWS_FICGTA01
    args.append("-vo");
    args.append("fbdev");
    args.append("-framedrop");
#endif
    if(filename.endsWith(".mp3", Qt::CaseInsensitive) ||
       filename.endsWith(".avi", Qt::CaseInsensitive))
    {
        args.append("-afm");
        args.append("ffmpeg");
        args.append("-vfm");
        args.append("ffmpeg");
    }

    if(filename.endsWith(".ogg", Qt::CaseInsensitive) ||
       filename.endsWith(".ogv", Qt::CaseInsensitive))
    {
        args.append("-afm");
        args.append("libvorbis");
    }

    args.append(filename);

    process = new QProcess(this);
    process->setProcessChannelMode(QProcess::ForwardedChannels);
    process->start("mplayer", args, QIODevice::ReadWrite);
}

void QMplayer::setRes(int xy)
{
    return;
#ifdef QT_QWS_FICGTA01
    if(xy == 320240 || xy == 640480)
    {
        QFile f("/sys/class/i2c-adapter/i2c-0/0-0073/pcf50633-regltr.9/glamo3362.0/glamo-spi-gpio.0/spi2.0/state");
        f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
        if(xy == 320240)
        {
            f.write("qvga-normal");

        }
        else if(xy == 640480)
        {
            f.write("normal");
        }
        f.close();
    }
#endif
}
