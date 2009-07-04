#include "qx.h"

QX::QX(QWidget *parent, Qt::WFlags f)
        : QWidget(parent)
{
#ifdef QT_QWS_FICGTA01
    this->setWindowState(Qt::WindowMaximized);
#else
    Q_UNUSED(f);
#endif
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
    is_fullscreen = false;
}

QX::~QX()
{

}

static void gpsPower(const char *powerStr)
{
#ifdef QT_QWS_FICGTA01
    QFile f("/sys/class/i2c-adapter/i2c-0/0-0073/pcf50633-regltr.7/neo1973-pm-gps.0/power_on");
    f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate);
    f.write(powerStr);
    f.close();
#endif
}

void QX::runApp(QString filename)
{
    enterFullScreen();
    bRun->setVisible(false);
    bTango->setVisible(false);
    bQuit->setVisible(false);
    lineEdit->setVisible(false);
    label->setVisible(true);
    label->setText(tr("Running") + " " + filename + "\n\n" + tr("5s touch will quit"));

    process = new QProcess(this);
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    process->setProcessChannelMode(QProcess::ForwardedChannels);
    process->start(filename, NULL);
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
        process->close();
        process = NULL;
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
        if(bQuit->isVisible())
        {
            bQuit->setVisible(false);
            system(QString("kill -CONT %1").arg(process->pid()).toAscii());
        }
        else
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
    bQuit->setVisible(true);
}

void QX::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    exitFullScreen();
    bRun->setVisible(true);
    bTango->setVisible(true);
    bQuit->setVisible(true);
    lineEdit->setVisible(true);
    label->setVisible(false);
    process = NULL;
}

bool QX::event(QEvent *event)
{
    if(event->type() == QEvent::WindowDeactivate && is_fullscreen)
    {
        lower();
    }
    else if(event->type() == QEvent::WindowActivate && is_fullscreen)
    {
        QString title = windowTitle();
        setWindowTitle(QLatin1String("_allow_on_top_"));
        raise();
        setWindowTitle(title);
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
    is_fullscreen = true;
#endif
}

void QX::exitFullScreen()
{
#ifdef QT_QWS_FICGTA01
    is_fullscreen = false;
    showMaximized();
#endif
}


