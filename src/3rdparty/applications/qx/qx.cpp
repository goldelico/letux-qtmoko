#include "qx.h"

QX::QX(QWidget *parent, Qt::WFlags f)
        : QWidget(parent)
{
    Q_UNUSED(f);

    bRun = new QPushButton(tr("Run"), this);
    connect(bRun, SIGNAL(clicked()), this, SLOT(runClicked()));

    bTango = new QPushButton(tr("Tango GPS"), this);
    connect(bTango, SIGNAL(clicked()), this, SLOT(tangoClicked()));

    bQuit = new QPushButton(tr("Quit"), this);
    connect(bQuit, SIGNAL(clicked()), this, SLOT(quitClicked()));

    label = new QLabel(this);
    label->setVisible(false);

    lineEdit = new QLineEdit("xclock", this);

    layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(lineEdit);
    layout->addWidget(bRun);
    layout->addWidget(bTango);
    layout->addWidget(bQuit);

    killTimer = new QTimer(this);
    killTimer->setSingleShot(true);
    connect(killTimer, SIGNAL(timeout()), this, SLOT(killTimerElapsed()));

    process = NULL;
    screen = QX::ScreenMain;

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
        exitFullScreen();
    }

    this->screen = scr;

    bRun->setVisible(scr == QX::ScreenMain);
    bTango->setVisible(scr == QX::ScreenMain);
    bQuit->setVisible(scr == QX::ScreenMain || scr == QX::ScreenPaused);
    lineEdit->setVisible(scr == QX::ScreenMain);
    label->setVisible(scr == QX::ScreenStarting || scr == QX::ScreenRunning);

    if(scr >= QX::ScreenFullscreen)
    {
        enterFullScreen();
    }
}

void QX::runApp(QString filename)
{
    label->setText(tr("Running") + " " + filename + "\n" + tr("5s press brings up menu"));
    showScreen(QX::ScreenStarting);

    process = new QProcess(this);
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    process->setProcessChannelMode(QProcess::ForwardedChannels);
    process->start(filename, NULL);

    if(process->waitForStarted())
    {
        showScreen(QX::ScreenRunning);
    }
    else
    {
        delete(process);
        process = NULL;
        showScreen(QX::ScreenMain);
        QMessageBox::critical(this, tr("QX"), tr("Unable to start") + " " + filename);
    }
}

void QX::runClicked()
{
    runApp(lineEdit->text());
}

void QX::tangoClicked()
{
#ifdef QT_QWS_FICGTA01
    gpsPower("1");
#endif
    system("gpsd /dev/ttySAC1");
    runApp("tangogps");
}

void QX::quitClicked()
{
    if(process)
    {
        process->kill();
    }
    else
    {
        close();
    }
}

void QX::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    if(killTimer->isActive())
    {
        killTimer->stop();
    }
    if(process)
    {
        if(screen == QX::ScreenPaused)
        {
            showScreen(QX::ScreenRunning);
            system(QString("kill -CONT %1").arg(process->pid()).toAscii());
        }
        else if(screen == QX::ScreenRunning)
        {
            killTimer->start(4000);
        }
    }
}

void QX::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    killTimer->stop();
}

void QX::killTimerElapsed()
{
    system(QString("kill -STOP %1").arg(process->pid()).toAscii());
    showScreen(QX::ScreenPaused);
}

void QX::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    showScreen(QX::ScreenMain);
    delete(process);
    process = NULL;
}

bool QX::event(QEvent *event)
{
    if(screen >= QX::ScreenFullscreen)
    {
        if(event->type() == QEvent::WindowDeactivate)
        {
            lower();
        }
        else if(event->type() == QEvent::WindowActivate)
        {
            QString title = windowTitle();
            setWindowTitle(QLatin1String("_allow_on_top_"));
            raise();
            setWindowTitle(title);
        }
    }
    return QWidget::event(event);
}

void QX::enterFullScreen()
{
#ifdef QT_QWS_FICGTA01
    // Show editor view in full screen
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    setWindowState(Qt::WindowFullScreen);
    raise();
#endif
}

void QX::exitFullScreen()
{
#ifdef QT_QWS_FICGTA01
    QTimer::singleShot(0, this, SLOT(showMaximized()));
#endif
}


