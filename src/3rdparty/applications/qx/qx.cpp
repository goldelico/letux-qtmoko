#include "qx.h"

QX::QX(QWidget *parent, Qt::WFlags f)
        : QWidget(parent)
{
    Q_UNUSED(f);

    bOk = new QPushButton(this);
    connect(bOk, SIGNAL(clicked()), this, SLOT(okClicked()));

    bTango = new QPushButton("Tango GPS", this);
    connect(bTango, SIGNAL(clicked()), this, SLOT(tangoClicked()));

    bScummvm = new QPushButton("ScummVM", this);
    connect(bScummvm, SIGNAL(clicked()), this, SLOT(scummvmClicked()));

    bQuit = new QPushButton(this);
    connect(bQuit, SIGNAL(clicked()), this, SLOT(quitClicked()));

    lineEdit = new QLineEdit("matchbox-session", this);

    layout = new QVBoxLayout(this);
    layout->addWidget(lineEdit);
    layout->addWidget(bOk);
    layout->addWidget(bTango);
    layout->addWidget(bScummvm);
    layout->addWidget(bQuit);

    appRunScr = new AppRunningScreen();

    process = NULL;
    xprocess = NULL;
    screen = QX::ScreenMain;
#if QT_QWS_FICGTA01
    powerConstraint = QtopiaApplication::Disable;

    // Start the "QX" service that handles application switching.
    new QxService(this);
#endif

    showScreen(QX::ScreenMain);
}

QX::~QX()
{

}

#ifdef QT_QWS_FICGTA01
static void gpsPower(const char *powerStr)
{
    QFile f("/sys/class/i2c-adapter/i2c-0/0-0073/pcf50633-regltr.7/neo1973-pm-gps.0/power_on");
    f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    f.write(powerStr);
    f.close();
}
#endif

void QX::showScreen(QX::Screen scr)
{
    if(scr < QX::ScreenFullscreen && this->screen >= QX::ScreenFullscreen)
    {
        appRunScr->hide();
        if(rotate)
        {
            system("xrandr -o 0");
        }
#ifdef QT_QWS_FICGTA01
        if(powerConstraint != QtopiaApplication::Enable)
        {
            QtopiaApplication::setPowerConstraint(QtopiaApplication::Enable);
        }
        QDeviceButtonManager &mgr = QDeviceButtonManager::instance();
        if(mgr.buttons().count() > 0)
        {
            mgr.remapPressedAction(0, origSrq);
        }
#endif
    }
    if(scr >= QX::ScreenFullscreen && this->screen < QX::ScreenFullscreen)
    {
        appRunScr->showScreen();
        if(rotate)
        {
            system("xrandr -o 1");
        }
#ifdef QT_QWS_FICGTA01
        if(powerConstraint != QtopiaApplication::Enable)
        {
            QtopiaApplication::setPowerConstraint(powerConstraint);
        }
        QDeviceButtonManager &mgr = QDeviceButtonManager::instance();
        if(mgr.buttons().count() > 0)
        {
            origSrq = mgr.buttons().at(0)->pressedAction();
            QtopiaServiceRequest req("QX", "appSwitch()");
            mgr.remapPressedAction(0, req);
        }
#endif
    }

    this->screen = scr;

    bOk->setVisible(scr == QX::ScreenMain || scr == QX::ScreenPaused);
    bQuit->setVisible(scr == QX::ScreenMain || scr == QX::ScreenPaused);
    bTango->setVisible(scr == QX::ScreenMain);
    bScummvm->setVisible(scr == QX::ScreenMain);
    lineEdit->setVisible(scr == QX::ScreenMain);

    switch(scr)
    {
    case QX::ScreenMain:
        bOk->setText(tr("Run"));
        bQuit->setText(tr("Quit"));
        update();
        break;
    case QX::ScreenPaused:
        bOk->setText(tr("Resume") + " " + appName);
        bQuit->setText(tr("Kill") + " " + appName);
        break;
    default:
        break;
    }
}

void QX::stopX()
{
    if(xprocess == NULL)
    {
        return;
    }
    if(xprocess->state() != QProcess::NotRunning)
    {
        xprocess->terminate();
        if(!xprocess->waitForFinished(3000))
        {
            xprocess->kill();
        }
        xprocess->waitForFinished();
    }
    delete(xprocess);
    xprocess = NULL;
}

void QX::runApp(QString filename, bool rotate)
{
    this->appName = filename;
    this->rotate = rotate;

    showScreen(QX::ScreenStarting);

    if(!QFile::exists("/tmp/.X0-lock"))
    {
        xprocess = new QProcess(this);
        xprocess->setProcessChannelMode(QProcess::ForwardedChannels);
        QStringList args;
        args.append("-hide-cursor");
        args.append("vt7");
        xprocess->start("X", args);
        if(!xprocess->waitForStarted())
        {
            showScreen(QX::ScreenMain);
            QMessageBox::critical(this, tr("QX"), tr("Unable to start X server"));
            return;
        }
    }

    Display *dpy;
    for(int i = 0; i < 3; i++)
    {
        dpy = XOpenDisplay(NULL);
        if(dpy != NULL)
        {
            break;
        }
        Sleeper::msleep(1000);
    }
    if(dpy == NULL)
    {
        stopX();
        showScreen(QX::ScreenMain);
        QMessageBox::critical(this, tr("QX"), tr("Unable to conntect to X server"));
        return;
    }
    XCloseDisplay(dpy);

    process = new QProcess(this);
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    process->setProcessChannelMode(QProcess::ForwardedChannels);
    process->start(filename, NULL);

    if(!process->waitForStarted())
    {
        delete(process);
        process = NULL;
        stopX();
        showScreen(QX::ScreenMain);
        QMessageBox::critical(this, tr("QX"), tr("Unable to start") + " " + filename);
        return;
    }

    showScreen(QX::ScreenRunning);
}

void QX::pauseApp()
{
    appRunScr->pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    system(QString("kill -STOP %1").arg(process->pid()).toAscii());
    if(xprocess)
    {
        system(QString("kill -STOP %1").arg(xprocess->pid()).toAscii());
    }
    showScreen(QX::ScreenPaused);
}

void QX::resumeApp()
{
    if(xprocess)
    {
        system(QString("kill -CONT %1").arg(xprocess->pid()).toAscii());
    }
    system(QString("kill -CONT %1").arg(process->pid()).toAscii());
    showScreen(QX::ScreenRunning);
}

void QX::okClicked()
{
    switch(screen)
    {
    case QX::ScreenMain:
        runApp(lineEdit->text(), false);
        break;
    case QX::ScreenPaused:
        resumeApp();
        break;
    default:
        break;
    }
}

void QX::tangoClicked()
{
#ifdef QT_QWS_FICGTA01
    gpsPower("1");
    powerConstraint = QtopiaApplication::DisableSuspend;
#endif
    system("gpsd /dev/ttySAC1");
    runApp("tangogps", false);
}

void QX::scummvmClicked()
{
#ifdef QT_QWS_FICGTA01
    powerConstraint = QtopiaApplication::Disable;
#endif
    runApp("/usr/games/scummvm", true);
}

void QX::quitClicked()
{
    if(process)
    {
        process->terminate();
        if(!process->waitForFinished(3000))
        {
            process->kill();
        }
    }
    else
    {
        close();
    }
}

void QX::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    showScreen(QX::ScreenMain);
    delete(process);
    process = NULL;
    stopX();
    appRunScr->pixmap = QPixmap();
#ifdef QT_QWS_FICGTA01
    powerConstraint = QtopiaApplication::Enable;
#endif
}


