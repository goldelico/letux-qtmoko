#include "qmplayer.h"

QMplayer::QMplayer(QWidget *parent, Qt::WFlags f)
    : QWidget(parent)
{
#ifdef QT_QWS_FICGTA01
    this->setWindowState(Qt::WindowMaximized);
#else
    Q_UNUSED(f);
#endif
    lw = new QListWidget(this);

    scanItem = new QListWidgetItem(tr("Scan"), lw);
    scanItem->setSelected(true);

    settingsItem = new QListWidgetItem(tr("Sharing"), lw);

    bOk = new QPushButton(this);
    connect(bOk, SIGNAL(clicked()), this, SLOT(okClicked()));

    bBack = new QPushButton(this);
    connect(bBack, SIGNAL(clicked()), this, SLOT(backClicked()));

    bUp = new QPushButton(this);
    connect(bUp, SIGNAL(clicked()), this, SLOT(upClicked()));

    bDown = new QPushButton(this);
    connect(bDown, SIGNAL(clicked()), this, SLOT(downClicked()));

    label = new QLabel(this);
    lineEdit = new QLineEdit(this);
    progress = new QProgressBar(this);    

    buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignBottom);
    buttonLayout->addWidget(bOk);
    buttonLayout->addWidget(bBack);
    buttonLayout->addWidget(bUp);
    buttonLayout->addWidget(bDown);

    layout = new QVBoxLayout(this);
    layout->addWidget(lw);
    layout->addWidget(label);
    layout->addWidget(lineEdit);
    layout->addWidget(progress);
    layout->addLayout(buttonLayout);

    maxScanLevel = 0;
    fbset = false;
    delTmpFiles = -1;
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
static bool processRunning(QProcess *p)
{
    return p != NULL &&
            (p->state() == QProcess::Running ||
             (p->state() == QProcess::Starting &&
              p->waitForStarted(1000)));
}

// Removes items from listview except top 2 items.
static void delItems(QListWidget *lw)
{
    while(lw->count() > 2)
    {
        delete(lw->takeItem(2));
    }
}

void QMplayer::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
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
        QStringList downList;
        QMessageBox::StandardButton answer = QMessageBox::NoButton;
        QMessageBox::StandardButton moreAnswer = QMessageBox::NoButton;
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
            if(!hit || isDir)
            {
                continue;
            }
            QString file = dir + "/" + path;
            if(!dir.startsWith("http://"))
            {
                list.append(file);      // add local files to playlist
                continue;
            }
            if(moreAnswer == QMessageBox::NoButton)
            {
                answer = QMessageBox::question(this, "qmplayer", file, QMessageBox::Save | QMessageBox::Open | QMessageBox::Cancel);
            }
            if(answer == QMessageBox::Cancel)
            {
                break;
            }
            if(answer == QMessageBox::Open)
            {
                list.append(file);      // append to playlist if we just play (no download)
            }
            downList.append(file);
            if(moreAnswer != QMessageBox::YesToAll)
            {
                moreAnswer = QMessageBox::question(this, "qmplayer", tr("Next file?"),
                                                   QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll);

                if(moreAnswer == QMessageBox::No)
                {
                    break;
                }
            }
        }
        for(int i = 0; i < downList.count(); i++)
        {
            QString url = downList[i];
            int slashIndex = url.lastIndexOf('/');
            if(slashIndex < 0)
            {
                continue;
            }
            QString filename = url.right(url.length() - slashIndex - 1);
            QString destPath = QDir::homePath() + "/" + filename;
            bool justCheck = list.contains(url);
            if(!download(url, destPath, filename, justCheck))
            {
                return;
            }
            if(justCheck)
            {
                continue;
            }
            // Add downloaded file to list
            list.append(destPath);
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
#ifdef QT_QWS_FICGTA01
            // Workaround unpause not working for alsa out in mplayer glamo.
            // We send left key to make mplayer start playing.
            process->write("\x1b""[D");
#endif
        }
        showScreen(QMplayer::ScreenPlay);
    }
    else if(screen == QMplayer::ScreenConnect)
    {
        runClient();
    }
}

bool QMplayer::download(QString url, QString destPath, QString filename, bool justCheck)
{
    QString host = url;
    QString reqPath;
    int port = 80;

    if(url.startsWith("http://"))
    {
        host.remove(0, 7);
    }

    int colonIndex = host.indexOf(':');
    int slashIndex = host.indexOf('/');
    if(slashIndex < 0)
    {
        return false;
    }
    reqPath = host.right(host.length() - slashIndex).replace(" ", "%20");
    host = host.left(slashIndex);
    if(colonIndex > 0)
    {
        QString portStr = host.right(host.length() - colonIndex - 1);
        host = host.left(colonIndex);
        port = portStr.toInt(0, 10);
    }

    QTcpSocket sock(this);
    sock.connectToHost(host, port);
    if(!sock.waitForConnected(5000))
    {
        QMessageBox::critical(this, tr("qmplayer"), sock.errorString());
        return false;
    }

    QByteArray req("GET ");
    req.append(reqPath);
    req.append(" HTTP/1.1\r\nHost: ");
    req.append(host);
    req.append(':');
    req.append(QByteArray::number(port));
    req.append("\r\n\r\n");

    sock.write(req);
    sock.flush();
    sock.waitForBytesWritten();

    int contentLen = 0;
    bool html = false;
    QByteArray line;
    for(;;)
    {
        line = sock.readLine();
        if(line.isEmpty())
        {
            if(sock.waitForReadyRead(5000))
            {
                continue;
            }
            break;
        }
        if(line.trimmed().isEmpty())
        {
            break;
        }
        html = html | (line.indexOf("Content-Type: text/html") == 0);
        if(line.indexOf("Content-Length: ") == 0)
        {
            contentLen = line.remove(0, 16).trimmed().toInt(0, 10);
        }
    }

    if(html)
    {
        QByteArray text = sock.readAll();
        if(text.length() == 0)
        {
            QMessageBox::critical(this, tr("qmplayer"),
                                  tr("No response from ") + host);
            sock.close();
            return false;
        }
        QMessageBox::information(this, tr("qmplayer"), text);
        sock.close();
        return false;
    }
    else if(justCheck)
    {
        sock.close();
        return true;
    }

    QFile f(destPath);
    if(!f.open(QFile::WriteOnly))
    {
        QMessageBox::critical(this, tr("qmplayer"),
                              tr("Unable to save file:\r\n\r\n") + f.errorString());
        sock.close();
        return false;
    }

    showScreen(QMplayer::ScreenDownload);
    label->setText(tr("Downloading") + " " + filename);

    if(contentLen <= 0)
    {
        contentLen = 1024 * 1024;
    }
    progress->setMaximum(contentLen);
    progress->setValue(0);
    int remains = contentLen;

    char buf[4096];
    int count;
    abort = false;
    for(;;)
    {
        QApplication::processEvents();
        if(abort)
        {
            break;
        }
        count = sock.read(buf, 4096);
        if(count > 0)
        {
            f.write(buf, count);
        }
        else if(!sock.waitForReadyRead(5000))
        {
            break;
        }
        remains -= count;
        if(remains <= 0)
        {
            remains = contentLen;
        }
        progress->setValue(contentLen - remains);
    }
    f.close();
    sock.close();

    return true;
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
    else if(screen == QMplayer::ScreenConnect)
    {
        showScreen(QMplayer::ScreenInit);
    }
    else
    {
        abort = true;
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

    lw->setVisible(scr == QMplayer::ScreenInit);
    bOk->setVisible(scr == QMplayer::ScreenInit || scr == QMplayer::ScreenPlay || scr == QMplayer::ScreenStopped || scr == QMplayer::ScreenConnect);
    bBack->setVisible(scr == QMplayer::ScreenInit || scr == QMplayer::ScreenPlay || scr == QMplayer::ScreenStopped || QMplayer::ScreenScan || scr == QMplayer::ScreenConnect);
    bUp->setVisible(scr == QMplayer::ScreenPlay || scr == QMplayer::ScreenStopped);
    bDown->setVisible(scr == QMplayer::ScreenPlay || scr == QMplayer::ScreenStopped);
    label->setVisible(scr == QMplayer::ScreenScan || scr == QMplayer::ScreenDownload || scr == QMplayer::ScreenConnect);
    lineEdit->setVisible(scr == QMplayer::ScreenConnect);
    progress->setVisible(scr == QMplayer::ScreenScan || scr == QMplayer::ScreenDownload);

    switch(scr)
    {
        case QMplayer::ScreenInit:
            bOk->setText(">");
            bBack->setText(tr("Quit"));
            break;
        case QMplayer::ScreenPlay:
            bOk->setText(tr("Pause"));
            bBack->setText(tr("Full screen"));
            bUp->setText(tr("Vol up"));
            bDown->setText(tr("Vol down"));
#ifdef QT_QWS_FICGTA01
            QtopiaApplication::setPowerConstraint(QtopiaApplication::Disable);
#endif
            break;
        case QMplayer::ScreenFullscreen:
#ifdef QT_QWS_FICGTA01
            setRes(320240);
#endif
            break;
        case QMplayer::ScreenStopped:
            bOk->setText("Play");
            bBack->setText(tr("Back"));
            bUp->setText(tr(">>"));
            bDown->setText(tr("<<"));
#ifdef QT_QWS_FICGTA01
            QtopiaApplication::setPowerConstraint(QtopiaApplication::Enable);
#endif
            break;
        case QMplayer::ScreenConnect:
            label->setText(tr("Enter host and port to connect to"));
            bOk->setText(tr("Ok"));
            bBack->setText(tr("Cancel"));
            break;
        case QMplayer::ScreenScan:
        case QMplayer::ScreenDownload:
            break;
    }
}

void QMplayer::scan()
{
    showScreen(QMplayer::ScreenScan);
    delItems(lw);
    abort = false;

    progress->setMinimum(0);
    progress->setMaximum(0x7fffffff);

scan_files:

#ifdef Q_WS_WIN
    scanDir("c:\\", 0, maxScanLevel, 0, 0x1fffffff);
#else
    // For the first time scan /home/root/Documents and dont scan other dirs if
    // something found there.
    if(maxScanLevel > 0 || !scanDir("/home/root/Documents", 0, 2, 0, 0x1fffffff))
    {
        scanDir("/", 0, 0, 0, 0x1fffffff);
        scanDir("/mnt", 0, maxScanLevel, 0, 0x2fffffff);
        scanDir("/media", 0, maxScanLevel, 0, 0x3fffffff);
        scanDir("/home", 0, maxScanLevel, 0, 0x4fffffff);
        scanDir("/home/root/Documents", 0, 2, 0, 0x5fffffff);
        scanDir("/root", 0, maxScanLevel, 0, 0x6fffffff);
    }
#endif

    maxScanLevel++;
    scanItem->setText(tr("Scan more"));

    if(lw->count() <= 2 && !abort &&
       QMessageBox::question(this, "qmplayer", tr("No media files found, scan more?"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        goto scan_files;
    }

    showScreen(QMplayer::ScreenInit);
}

int QMplayer::scanDir(QString const& path, int level, int maxLevel, int min, int max)
{
    if(abort)
    {
        return 0;
    }

    QDir dir(path);
    QFileInfoList list = dir.entryInfoList(QDir::AllEntries, QDir::Name);

    int found = 0;
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
            if(fileName.contains(".qmplayer."))
            {
                if(delTmpFiles < 0)
                {
                    delTmpFiles = (QMessageBox::question(this, "qmplayer", tr("Delete qmplayer temporary files?"),
                                                        QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes);
                }
                if(delTmpFiles)
                {
                    QFile::remove(fileName);
                }
                continue;
            }
            QListWidgetItem *fileItem = new QListWidgetItem(fileName);
            fileItem->setTextAlignment(Qt::AlignHCenter);
            lw->addItem(fileItem);
            found++;
        }
    }

    if(found)
    {
        lw->insertItem(index, path);
    }

    if(level >= maxLevel)
    {
        return found;
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
        label->setText(fi.absolutePath());
        QApplication::processEvents();

        found += scanDir(fi.filePath(), level + 1, maxLevel, value, value + unit);
    }
    return found;
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
                             tr("Web server is running on port 7654"));

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

    if(screen != QMplayer::ScreenConnect)
    {
        lineEdit->setText("192.168.0.200:7654");
        showScreen(QMplayer::ScreenConnect);
        return true;
    }
    showScreen(QMplayer::ScreenInit);

    QString host = lineEdit->text();
    int port = 80;
    int colonIndex = host.indexOf(':');
    if(colonIndex > 0)
    {
        port = host.right(host.length() - colonIndex - 1).toInt(0, 10);
        host = host.remove(colonIndex, host.length() - colonIndex);
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

    delItems(lw);

    QBuffer buf(&res);
    QByteArray line;
    QString dir;

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
            for(int i = fileStart - 1; i >= 0; i--)
            {
                if(dir.length() <= i || dir.at(i) != url.at(i))
                {
                    dir = url.left(fileStart);
                    lw->addItem(dir);
                    break;
                }
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
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
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

void QMplayer::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    showScreen(QMplayer::ScreenInit);
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
#else
    Q_UNUSED(xy);
#endif
}

bool QMplayer::installMplayer()
{
#ifdef QT_QWS_FICGTA01
    QDir("/home/root").mkdir(".mplayer");
    QFile f("/home/root/.mplayer/config");
    f.open(QFile::WriteOnly);
    f.write("vo=glamo\n\n[default]\nafm=ffmpeg\nvfm=ffmpeg\n");
    f.close();

    return download("http://72.249.85.183/radekp/qmplayer/download/mplayer",
                    "/usr/bin/mplayer", "mplayer", false) &&
    QFile::setPermissions("/usr/bin/mplayer", QFile::ReadOwner |
                          QFile::WriteOwner | QFile::ExeOwner |
                          QFile::ReadUser | QFile::ExeUser |
                          QFile::ReadGroup | QFile::ExeGroup |
                          QFile::ReadOther | QFile::ExeOther);
#else
    QMessageBox::critical(this, tr("qmplayer"), tr("You must install mplayer"));
    return false;
#endif
}
