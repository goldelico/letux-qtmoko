#include "qmplayer.h"

QMplayer::QMplayer(QWidget *parent, Qt::WFlags f)
    : QWidget(parent)
{
#ifdef QT_QWS_FICGTA01
    this->setWindowState(Qt::WindowMaximized);
#endif
    lw = new QListWidget(this);

    scanItem = new QListWidgetItem(tr("Scan"), lw);
    scanItem->setSelected(true);

    settingsItem = new QListWidgetItem(tr("Settings"), lw);

    bOk = new QPushButton(">", this);
    connect(bOk, SIGNAL(clicked()), this, SLOT(okClicked()));

    bBack = new QPushButton("Back", this);
    connect(bBack, SIGNAL(clicked()), this, SLOT(backClicked()));

    bUp = new QPushButton("Vol up", this);
    connect(bUp, SIGNAL(clicked()), this, SLOT(upClicked()));

    bDown = new QPushButton("Vol down", this);
    connect(bDown, SIGNAL(clicked()), this, SLOT(downClicked()));

    label = new QLabel(this);
    progress = new QProgressBar(this);

    buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignBottom);
    buttonLayout->addWidget(bOk);
    buttonLayout->addWidget(bBack);
    buttonLayout->addWidget(bUp);
    buttonLayout->addWidget(bDown);
    buttonLayout->addWidget(label);
    buttonLayout->addWidget(progress);

    layout = new QVBoxLayout(this);
    layout->addWidget(lw);
    layout->addLayout(buttonLayout);

    maxScanLevel = 0;
    fbset = false;

    showScreen(QMplayer::ScreenInit);
}

QMplayer::~QMplayer()
{

}

void QMplayer::mousePressEvent(QMouseEvent * event)
{
    if(screen == QMplayer::ScreenFullscreen)
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
        QString dir = "";
        bool hit = false;
        QStringList list;
        for(int i = 2; i < lw->count(); i++)
        {
            QListWidgetItem *item = lw->item(i);
            QString path = item->text();
            bool isDir = path.startsWith('/');
            if(isDir)
            {
                if(hit)
                {
                    break;
                }
                dir = path;
            }
            hit |= (item == sel);
            if(hit && !isDir)
            {
                list.append(dir + "/" + path);
            }
        }
        if(list.count() > 0)
        {
            play(list);
        }
    }
    else if(screen == QMplayer::ScreenPlay)
    {
        if(process != NULL && (process->state() == QProcess::Running))
        {
            process->write(" ");
        }
        showScreen(QMplayer::ScreenStopped);
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
    else if(screen == QMplayer::ScreenPlay)
    {
        showScreen(QMplayer::ScreenFullscreen);
    }
    else if(screen == QMplayer::ScreenStopped)
    {
        process->write("q");
        process->waitForFinished(4000);
        delete(process);
        showScreen(QMplayer::ScreenInit);
    }
}

void QMplayer::upClicked()
{
    if(process != NULL && (process->state() == QProcess::Running))
    {
        if(screen == QMplayer::ScreenStopped)
        {
            process->write("\x1b""[C ");
        }
        else
        {
            process->write("0");
        }
    }
}

void QMplayer::downClicked()
{
    if(process != NULL && (process->state() == QProcess::Running))
    {
        if(screen == QMplayer::ScreenStopped)
        {
            process->write("\x1b""[D ");
        }
        else
        {
            process->write("9");
        }
    }
}

void QMplayer::showScreen(QMplayer::Screen scr)
{
    if(screen == QMplayer::ScreenFullscreen)
    {
        setRes(640480);
    }

    this->screen = scr;

    lw->setVisible(scr == QMplayer::ScreenInit || scr == QMplayer::ScreenMplayerInstall);
    bOk->setVisible(scr == QMplayer::ScreenInit || scr == QMplayer::ScreenPlay || scr == QMplayer::ScreenStopped);
    bBack->setVisible(scr == QMplayer::ScreenInit || scr == QMplayer::ScreenPlay || scr == QMplayer::ScreenStopped);
    bUp->setVisible(scr == QMplayer::ScreenPlay || scr == QMplayer::ScreenStopped);
    bDown->setVisible(scr == QMplayer::ScreenPlay || scr == QMplayer::ScreenStopped);
    label->setVisible(scr == QMplayer::ScreenScan);
    progress->setVisible(scr == QMplayer::ScreenScan);

    if(scr == QMplayer::ScreenInit)
    {
        bBack->setText(tr("Quit"));
    }
    else if(scr == QMplayer::ScreenPlay)
    {
        bOk->setText(tr("Pause"));
        bBack->setText(tr("Full screen"));
        bUp->setText(tr("Vol up"));
        bDown->setText(tr("Vol down"));
    }
    else if(scr == QMplayer::ScreenFullscreen)
    {
#ifdef QT_QWS_FICGTA01
        setRes(320240);
#endif
    }
    else if(scr == QMplayer::ScreenStopped)
    {
        bOk->setText("Play");
        bBack->setText(tr("Back"));
        bUp->setText(tr(">>"));
        bDown->setText(tr("<<"));
    }
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

    scanDir("/", 0, 0, 0, 0x1fffffff);
    scanDir("/mnt", 0, maxScanLevel, 0, 0x2fffffff);
    scanDir("/media", 0, maxScanLevel, 0, 0x3fffffff);
    scanDir("/home", 0, maxScanLevel, 0, 0x4fffffff);
    scanDir("/home/root/Documents", 0, maxScanLevel + 2, 0, 0x5fffffff);
    scanDir("/root", 0, maxScanLevel, 0, 0x6fffffff);

    maxScanLevel++;
    scanItem->setText(tr("Scan more"));

    showScreen(QMplayer::ScreenInit);
}

void QMplayer::scanDir(QString const& path, int level, int maxLevel, int min, int max)
{
    QDir dir(path);
    QFileInfoList list = dir.entryInfoList(QDir::AllEntries, QDir::Name);

    bool found = false;
    int index = lw->count();
    for(int i = 0; i < list.count(); i++)
    {
        QFileInfo fi = list.at(i);
        if(!fi.isFile())
        {
            continue;
        }
        QString fileName = fi.fileName();
        if(fileName.endsWith(".mp3", Qt::CaseInsensitive)
            || fileName.endsWith(".ogg", Qt::CaseInsensitive)
            || fileName.endsWith(".ogv", Qt::CaseInsensitive)
            || fileName.endsWith(".avi", Qt::CaseInsensitive))
        {
            QListWidgetItem *fileItem = new QListWidgetItem(fileName);
            fileItem->setTextAlignment(Qt::AlignHCenter);
            lw->addItem(fileItem);
            found = true;
        }
    }

    if(found)
    {
        lw->insertItem(index, path);
    }

    if(level >= maxLevel)
    {
        return;
    }

    for(int i = 0; i < list.count(); i++)
    {
        QFileInfo fi = list.at(i);
        if(fi.isFile() || fi.fileName() == "." || fi.fileName() == "..")
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
        scanDir(fi.filePath(), level + 1, maxLevel, value, value + unit);
    }
}

void QMplayer::settings()
{
}

void QMplayer::play(QStringList const& args)
{
    showScreen(QMplayer::ScreenPlay);

    process = new QProcess(this);
    process->setProcessChannelMode(QProcess::ForwardedChannels);
    process->start("mplayer", args, QIODevice::ReadWrite);

    if(!process->waitForStarted(5000))
    {
       delete(process);
       process = NULL;

       if(QMessageBox::question(this, tr("qmplayer"),
                             tr("Program MPlayer must be downloaded. Please make sure you have internet connection and press yes to confirm download"),
                             QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
       {
           if(installMplayer())
           {
               QMessageBox::information(this, tr("qmplayer"), tr("MPlayer installed sucessfully"));
               play(args);
               return;
           }
           QMessageBox::warning(this, tr("qmplayer"), tr("Failed to install MPlayer"));
           showScreen(QMplayer::ScreenInit);
       }
       return;
    }
#ifdef QT_QWS_FICGTA01
    QtopiaApplication::setPowerConstraint(QtopiaApplication::Disable);
#endif
}

void QMplayer::setRes(int xy)
{
#ifdef QT_QWS_FICGTA01
    if(xy == 320240 || xy == 640480)
    {
        QFile f("/sys/bus/spi/devices/spi2.0/state");
        f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
        if(xy == 320240)
        {
            if(!fbset)
            {
                fbset = true;
                QProcess p(this);
                p.start("fbset", QStringList("vga"));
                p.waitForFinished(5000);
            }
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

bool QMplayer::runProcess(QString const& info, QProcess *p, QString const& program, QStringList const& args)
{
    while(lw->count() > 2)
    {
        delete(lw->takeItem(2));
    }

    scanItem->setText(info);
    QString argText(program);
    settingsItem->setText(program);
    for(int i = 0; i < args.count(); i++)
    {
        argText.append(' ');
        argText.append(args[i]);
    }
    settingsItem->setText(argText);

    p->start(program, args);

    if(!p->waitForStarted(5000))
    {
        QMessageBox::warning(this, tr("qmplayer"), tr("Failed to start ") + program);
        return false;
    }

    QString str("");
    for(;;)
    {
        p->waitForFinished(100);
        bool finished = (p->state() != QProcess::Running);

        str += p->readAllStandardOutput();
        str += p->readAllStandardError();
        for(int i = 0; i < str.length(); i++)
        {
            if(str.at(i) != '\n')
            {
                continue;
            }
            lw->addItem(str.left(i));
            str.remove(0, i + 1);
            lw->scrollToBottom();
        }
        QApplication::processEvents();
        QApplication::processEvents();
        QApplication::processEvents();

        if(finished)
        {
            break;
        }
    }

    return true;
}

bool QMplayer::installMplayer()
{
    QProcess wget;
    QStringList wgetArgs;
    QProcess tar;
    QStringList tarArgs;

    showScreen(QMplayer::ScreenMplayerInstall);

    QFile::remove("/tmp/mplayer_glamo_install.tar.gz");

    // "http://activationrecord.net/radekp/openmoko/qtmoko/download/mplayer_glamo_install.tar.gz
    wgetArgs.append("http://72.249.85.183/radekp/openmoko/qtmoko/download/mplayer_glamo_install.tar.gz");
    wget.setWorkingDirectory("/tmp");
    if(!runProcess(tr("Downloading MPlayer"), &wget, "wget", wgetArgs))
    {
        return false;
    }

    tarArgs.append("xzvpf");
    tarArgs.append("/tmp/mplayer_glamo_install.tar.gz");
    tar.setWorkingDirectory("/");

    if(!runProcess(tr("Unpacking MPlayer"), &tar, "tar", tarArgs))
    {
        return false;
    }

    QFile::remove("/tmp/mplayer_glamo_install.tar.gz");

    return QFile::exists("/usr/bin/mplayer");
}
