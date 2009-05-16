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

    settingsItem = new QListWidgetItem(tr("Web sharing"), lw);

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
    process = NULL;
    tcpServer = NULL;

    showScreen(QMplayer::ScreenInit);
}

QMplayer::~QMplayer()
{

}

static bool isDirectory(QString path)
{
    return path != NULL && (
#ifdef Q_WS_WIN
    (path.length() >= 3) && (path.at(1) == ':')
#else
    path.startsWith('/')
#endif
    || path.startsWith("http://"));
}

// Return directory suitable for placing as url
static QString dirToUrl(QString dir)
{
    QString res(dir);
    for(int i = 0; i < res.length(); i++)
    {
        QChar ch = res.at(i);
        if(ch == ':')
        {
            res[i] = '_';
        }
        else if(ch == '\\')
        {
            res[i] = '/';
        }
    }
    while(res.endsWith('/') && res.length() > 0)
    {
        res.remove(res.length() - 1, 1);
    }
    if(res.at(0) != '/')
    {
        res.prepend('/');
    }
    return res;
}

// Returns if process is running. If process is starting then waits for start
// and returns start result.
bool processRunning(QProcess *p)
{
    return p != NULL &&
            (p->state() == QProcess::Running ||
             (p->state() == QProcess::Starting &&
              p->waitForStarted(1000)));
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
            bool isDir = isDirectory(path);
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
        if(processRunning(process))
        {
            process->write(" ");
        }
        showScreen(QMplayer::ScreenStopped);
    }
    else if(screen == QMplayer::ScreenStopped)
    {
        if(processRunning(process))
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
        process = NULL;
        showScreen(QMplayer::ScreenInit);
    }
}

void QMplayer::upClicked()
{
    if(processRunning(process))
    {
        if(screen == QMplayer::ScreenStopped)
        {
            process->write("\x1b""[A ");
        }
        else
        {
            process->write("0");
        }
    }
}

void QMplayer::downClicked()
{
    if(processRunning(process))
    {
        if(screen == QMplayer::ScreenStopped)
        {
            process->write("\x1b""[B ");
        }
        else
        {
            process->write("9");
        }
    }
}

void QMplayer::showScreen(QMplayer::Screen scr)
{
    // Full screen -> normal
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

#ifdef QT_QWS_FICGTA01
    if(scr == QMplayer::ScreenPlay)
    {
        QtopiaApplication::setPowerConstraint(QtopiaApplication::Disable);
    }
    if(scr == QMplayer::ScreenStopped)
    {
        QtopiaApplication::setPowerConstraint(QtopiaApplication::Enable);
    }
#endif
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

#ifdef Q_WS_WIN
    scanDir("c:\\", 0, 0, 0, 0x1fffffff);
#else
    scanDir("/", 0, 0, 0, 0x1fffffff);
    scanDir("/mnt", 0, maxScanLevel, 0, 0x2fffffff);
    scanDir("/media", 0, maxScanLevel, 0, 0x3fffffff);
    scanDir("/home", 0, maxScanLevel, 0, 0x4fffffff);
    scanDir("/home/root/Documents", 0, maxScanLevel + 2, 0, 0x5fffffff);
    scanDir("/root", 0, maxScanLevel, 0, 0x6fffffff);
#endif

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

bool QMplayer::runServer()
{
    if(tcpServer != NULL)
    {
        return false;
    }
    if(QMessageBox::question(this, "qmplayer", tr("Start server?"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
    {
        return false;
    }

    tcpServer = new QTcpServer(this);

    if(!tcpServer->listen(QHostAddress::Any, 7654))
    {
        QMessageBox::critical(this, tr("qmplayer"),
                              tr("Unable to start the server on port 7654: ") + tcpServer->errorString());
        delete(tcpServer);
        tcpServer = NULL;
        return true;
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

    QMessageBox::information(this, tr("qmplayer"),
                             tr("The server is running on port 7654"));

    settingsItem->setText(tr("Web server running on port 7654"));
    return true;
}

bool QMplayer::runClient()
{
    if(tcpServer != NULL)
    {
        QMessageBox::critical(this, tr("qmplayer"),
                              tr("Point your web browser to http://localhost:7654 (replace localhost with this computer IP address)"));
        return true;
    }

    bool ok = true;

    QString host = "192.168.0.200";
    //QString host = QInputDialog::getText(this, "qmplayer", tr("Host to connect to:"),
    //                                     QLineEdit::Normal, "192.168.0.200", &ok);

    if(!ok)
    {
        return false;
    }

    int port = 7654;
    //int port = QInputDialog::getInteger(this, "qmplayer", tr("port:"), 7654, 0,
    //                                    65535, 1, &ok);
    
    if(!ok)
    {
        return false;
    }

    QTcpSocket sock(this);
    sock.connectToHost(host, port);
    if(!sock.waitForConnected(5000))
    {
        QMessageBox::critical(this, tr("qmplayer"), sock.errorString());
        return true;
    }

    QByteArray req("GET / HTTP/1.1\r\nHost: ");
    req.append(host);
    req.append(':');
    req.append(QByteArray::number(port));
    req.append("\r\n");

    sock.write(req);
    sock.flush();
    sock.waitForBytesWritten();

    QByteArray res;
    while(sock.waitForReadyRead(5000))
    {
        res += sock.readAll();
    }
    sock.close();

    if(res.length() == 0)
    {
        QMessageBox::critical(this, tr("qmplayer"),
                              tr("No response from ") + host);
        return true;
    }

    QBuffer buf(&res);
    QByteArray line;
    QString dir = ":";

    buf.open(QBuffer::ReadOnly);
    for(;;)
    {
        line = buf.readLine();
        if(line.isEmpty())
        {
            break;
        }
        if(line.indexOf("<a href=\"http://") == 0)
        {
            int urlEnd = line.indexOf('"', 17);
            if(urlEnd < 0)
            {
                continue;
            }
            QString url = line.mid(9, urlEnd - 9);
            int fileStart = url.lastIndexOf('/');
            if(fileStart < 0)
            {
                continue;
            }
            if(!url.startsWith(dir))
            {
                dir = url.left(fileStart);
                lw->addItem(dir);
            }
            QString file = url.right(url.length() - fileStart - 1);
            QListWidgetItem *item = new QListWidgetItem(file);
            item->setTextAlignment(Qt::AlignHCenter);
            lw->addItem(item);
        }
    }

    return true;
}

void QMplayer::settings()
{
#ifdef QT_QWS_FICGTA01
    if(!runClient())
    {
        runServer();
    }
#else
    if(!runServer())
    {
        runClient();
    }
#endif
}

void QMplayer::newConnection()
{
    QTcpSocket *con = tcpServer->nextPendingConnection();
    connect(con, SIGNAL(disconnected()),
            con, SLOT(deleteLater()));

    QByteArray req;
    while(con->waitForReadyRead(100))
    {
        req += con->readAll();
    }

    QByteArray res;
    QBuffer buf(&req);
    QByteArray line;
    QString reqPath = "";           // path from http request
    QString filename = "";          // file on the disk
    QString encPath = "";           // transcoded file
    QString host = "";
    QFile file;
    bool sendFile = false;
    int itemIndex = 0;

    buf.open(QBuffer::ReadOnly);
    for(;;)
    {
        line = buf.readLine();
        if(line.isEmpty())
        {
            break;
        }
        if(line.startsWith("GET "))
        {
            int index = req.indexOf(' ', 4);
            if(index > 0)
            {
                reqPath = req.mid(4, index - 4);
                for(int i = 0; i < reqPath.count(); i++)
                {
                    if(reqPath.at(i) == '%')
                    {
                        i += 2;
                        if(i >= reqPath.count())
                        {
                            break;
                        }
                        QChar ch = (QChar)(reqPath.mid(i - 1, 2).toInt(0, 16));
                        reqPath.replace(i - 2, 3, ch);
                    }
                }
            }
        }
        else if(line.startsWith("Host: "))
        {
            host = line.right(line.length() - 6).trimmed();
        }
    }

    res.append("HTTP/1.0 200 OK\r\n");
    res.append("Content-Type: text/html; charset=utf-8\r\n\r\n");
    res.append("<html>");
    res.append("<body>");

    if(reqPath.length() == 0)
    {
        res.append("Valid GET request not found");
    }
    else if(host.length() == 0)
    {
        res.append("Host not found in html header");
    }
    else if(reqPath == "/")
    {
        QString dir = "";
        for(int i = 2; i < lw->count(); i++)
        {
            QListWidgetItem *item = lw->item(i);
            QString path = item->text();
            bool isDir = isDirectory(path);
            if(isDir)
            {
                res.append(path);
                res.append("</br>");
                dir = dirToUrl(path);
            }
            else
            {
                res.append("\r\n<a href=\"http://");
                res.append(host);
                res.append(dir);
                res.append('/');
                res.append(path);
                res.append("\">");
                res.append(path);
                res.append("</a></br>\r\n");
            }
        }
    }
    else
    {
        res.append(reqPath);
        res.append("</br>");
        QString dir = "";
        bool ok = false;
        for(int i = 2; i < lw->count(); i++)
        {
            QListWidgetItem *item = lw->item(i);
            QString path = item->text();
            bool isDir = isDirectory(path);
            if(isDir)
            {
                dir = path;
            }
            else if(reqPath.endsWith(path))
            {
                QString testPath = dirToUrl(dir) + '/' + path;
                if(!ok || reqPath == testPath)
                {
                    filename = dir;
                    if(!filename.endsWith('/') && !filename.endsWith('\\'))
                    {
                        filename.append('/');
                    }
                    filename.append(path);
                    itemIndex = i;
                    ok = true;
                }
            }
        }
        if(!ok)
        {
            res.append("file not found");
        }
        else
        {
            encPath = filename;
            // Encode just avi files for now
            if(encPath.endsWith(".avi"))
            {
                encPath.insert(encPath.lastIndexOf('.'), ".qmplayer");
            }
            QFileInfo fi(encPath);
            if(fi.exists())
            {
                QString size;
                size.setNum(fi.size());
                res.append("file size: ");
                res.append(size);

                if(processRunning(process))
                {
                    res.append("</br>encoding in progress</br>Reload page to check status or download");
                }
                else
                {
                    QString mime = "text/plain";
                    if(reqPath.endsWith(".avi"))
                    {
                        mime ="video/avi";
                    }
                    else if(reqPath.endsWith(".ogv"))
                    {
                        mime = "video/theora";
                    }
                    else if(reqPath.endsWith(".ogg"))
                    {
                        mime = "audio/vorbis";
                    }
                    else if(reqPath.endsWith(".mp3"))
                    {
                        mime = "audio/mpeg";
                    }

                    res.clear();
                    res.append("HTTP/1.0 200 OK\r\n");
                    res.append("Content-Type: ");
                    res.append(mime);
                    res.append("\r\nContent-Length: ");
                    res.append(size);
                    res.append("\r\n\r\n");

                    sendFile = true;
                }
            }
            else if(processRunning(process))
            {
                res.append("Another encoding is in progress");
            }
            else
            {
                QStringList args;
                args.append(filename);
                args.append("-ovc");
                args.append("lavc");
                args.append("-lavcopts");
                args.append("vcodec=mpeg4:vhq:vbitrate=300");
                args.append("-vf");
                args.append("scale=320:240,eq2=1.2:0.5:-0.25,rotate=2");
                args.append("-oac");
                args.append("mp3lame");
                args.append("-ofps");
                args.append("15");
                args.append("-lameopts");
                args.append("br=64:cbr");
                args.append("-o");
                args.append(encPath);

                process = new QProcess(this);
                process->setProcessChannelMode(QProcess::ForwardedChannels);
                process->start("mencoder", args);
#ifdef Q_WS_WIN
                PROCESS_INFORMATION *pi = (PROCESS_INFORMATION *) process->pid();
                SetPriorityClass(pi->hProcess, IDLE_PRIORITY_CLASS);
#endif
                // FIXME: whatever we send to back, browser wont show anything
                if(process->waitForStarted(10000))
                {
                    res.append("Encoding started</br>Reload page to check status or download");
                }
                else
                {
                    res.append("Failed to start mencoder:</br>");
                    res.append(process->errorString());
                    delete(process);
                    process = NULL;
                }
            }
        }
    }

    if(!sendFile)
    {
        res.append("</body>");
        res.append("</html>");
    }
    con->write(res);

    if(sendFile)
    {
        QFile f(encPath);
        char buf[4096];
        f.open(QFile::ReadOnly);
        int count;
        while((count = f.read(buf, 4096)) > 0)
        {
            con->write(buf, count);
            con->flush();
        }
    }

    con->close();
    con->disconnectFromHost();
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
