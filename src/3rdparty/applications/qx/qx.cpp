#include "qx.h"

QX::QX(QWidget *parent, Qt::WFlags f)
        : QWidget(parent)
{
    Q_UNUSED(f);

    bOk = new QPushButton(this);
    connect(bOk, SIGNAL(clicked()), this, SLOT(okClicked()));

    bTango = new QPushButton("Tango GPS", this);
    connect(bTango, SIGNAL(clicked()), this, SLOT(tangoClicked()));

    bQuit = new QPushButton(this);
    connect(bQuit, SIGNAL(clicked()), this, SLOT(quitClicked()));

    lineEdit = new QLineEdit("xclock", this);

    layout = new QVBoxLayout(this);
    layout->addWidget(lineEdit);
    layout->addWidget(bOk);
    layout->addWidget(bTango);
    layout->addWidget(bQuit);

    appRunScr = new AppRunningScreen();
    connect(appRunScr, SIGNAL(longPress()), this, SLOT(pauseApp()));

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
        appRunScr->hide();
    }
    if(scr >= QX::ScreenFullscreen && this->screen < QX::ScreenFullscreen)
    {
        appRunScr->showScreen();
    }

    this->screen = scr;

    bOk->setVisible(scr == QX::ScreenMain || scr == QX::ScreenPaused);
    bQuit->setVisible(scr == QX::ScreenMain || scr == QX::ScreenPaused);
    bTango->setVisible(scr == QX::ScreenMain);
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
    }
}

void QX::runApp(QString filename)
{
    this->appName = filename;
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

void QX::pauseApp()
{
    appRunScr->pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
    system(QString("kill -STOP %1").arg(process->pid()).toAscii());
    showScreen(QX::ScreenPaused);
}

void QX::resumeApp()
{
    system(QString("kill -CONT %1").arg(process->pid()).toAscii());
    showScreen(QX::ScreenRunning);
}

void QX::okClicked()
{
    switch(screen)
    {
    case QX::ScreenMain:
        runApp(lineEdit->text());
        break;
    case QX::ScreenPaused:
        resumeApp();
        break;
    }
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

void QX::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    showScreen(QX::ScreenMain);
    delete(process);
    process = NULL;
    appRunScr->pixmap.fill(Qt::black);
}


