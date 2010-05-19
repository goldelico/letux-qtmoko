#include "neocontrol.h"

NeoControl::NeoControl(QWidget *parent, Qt::WFlags f)
    : QWidget(parent)
{
#ifdef QTOPIA
    this->setWindowState(Qt::WindowMaximized);
#else
    Q_UNUSED(f);
#endif
    bQvga = new QPushButton(tr("Switch to QVGA"), this);
    connect(bQvga, SIGNAL(clicked()), this, SLOT(qvgaClicked()));

    bBack = new QPushButton(this);
    connect(bBack, SIGNAL(clicked()), this, SLOT(backClicked()));

    bNext = new QPushButton(tr("Next"), this);
    connect(bNext, SIGNAL(clicked()), this, SLOT(nextClicked()));

    chkDeepSleep = new QCheckBox(tr("Deep sleep"), this);
    connect(chkDeepSleep, SIGNAL(stateChanged(int)), this, SLOT(deepSleepStateChanged(int)));

    chkMux = new QCheckBox(tr("Multiplexing"), this);
    connect(chkMux, SIGNAL(stateChanged(int)), this, SLOT(muxStateChanged(int)));

    label = new QLabel(this);
    lineEdit = new QLineEdit(this);

    buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignBottom);
    buttonLayout->addWidget(bBack);
    buttonLayout->addWidget(bNext);

    layout = new QVBoxLayout(this);
    layout->addWidget(bQvga);
    layout->addWidget(label);
    layout->addWidget(lineEdit);
    layout->addWidget(chkDeepSleep);
    layout->addWidget(chkMux);
    layout->addLayout(buttonLayout);

    showScreen(NeoControl::ScreenInit);
}

NeoControl::~NeoControl()
{

}

void NeoControl::backClicked()
{
    switch(screen)
    {
    case ScreenInit:
        close();
        break;
    case ScreenModem:
        showScreen(ScreenInit);
        break;
    case ScreenSysfs:
        showScreen(ScreenModem);
        break;
    case ScreenDisplay:
        showScreen(ScreenSysfs);
        break;
    }
}

void NeoControl::nextClicked()
{
    switch(screen)
    {
    case ScreenInit:
        showScreen(ScreenModem);
        break;
    case ScreenModem:
        showScreen(ScreenSysfs);
        break;
    case ScreenSysfs:
        showScreen(ScreenDisplay);
        break;
    case ScreenDisplay:
        break;
    }
}

void NeoControl::qvgaClicked()
{
    QFile cal("/etc/pointercal_qvga");
    if(!cal.exists())
    {
        cal.open(QFile::WriteOnly);
        cal.write("-360 16920 -2227050 -21726 -170 20270704 56321");
        cal.close();
    }

    QFile f("/tmp/restart-qtopia-qvga");
    f.open(QFile::WriteOnly);
    f.write("restart in qvga");
    f.close();
    QMessageBox::information(this, tr("QVGA setup"), tr("QVGA mode will be activated after restarting QtExtended with POWER button"));
}

void NeoControl::showScreen(NeoControl::Screen scr)
{
    this->screen = scr;

    bQvga->setVisible(scr == ScreenDisplay);
    label->setVisible(scr == ScreenInit || scr == ScreenModem || scr == ScreenSysfs);
    bBack->setText(scr == ScreenInit ? tr("Quit") : tr("Back"));
    lineEdit->setVisible(false);
    chkDeepSleep->setVisible(scr == ScreenModem);
    chkMux->setVisible(scr == ScreenModem);

    switch(scr)
    {
    case ScreenInit:
        label->setText(tr("Neo hardware tool"));
        break;
    case ScreenModem:
        updateModem();
        break;
    case ScreenDisplay:
        break;
    case ScreenSysfs:
        updateSysfs();
        break;

    default:
        break;
    }
}

void NeoControl::deepSleepStateChanged(int state)
{
    if(updatingModem)
    {
        return;
    }
    QString val = (state == Qt::Checked ? "always" : "never");
    QSettings cfg("Trolltech", "Modem");
    cfg.setValue("DeepSleep/Active", val);
    cfg.sync();

    QMessageBox::information(this, tr("Deep sleep"), tr("Settings will be activated after restarting QtExtended with POWER button"));
}

void NeoControl::muxStateChanged(int state)
{
    if(updatingModem)
    {
        return;
    }
    QString val = (state == Qt::Checked ? "yes" : "no");
    QSettings cfg("Trolltech", "Modem");
    cfg.setValue("Multiplexing/Active", val);
    cfg.sync();

    QMessageBox::information(this, tr("Multiplexing"), tr("Settings will be activated after restarting QtExtended with POWER button"));
}

void NeoControl::updateModem()
{
    if(screen != ScreenModem)
    {
        return;
    }
    updatingModem = true;

    QString text(tr("Modem settings\n\n"));
    QSettings cfg("Trolltech", "Modem");

    QString deepSleep = cfg.value("DeepSleep/Active").toString();
    text += tr("Deep sleep") + ": " + deepSleep + "\n";
    chkDeepSleep->setChecked(deepSleep != "never");

    QString multiplexing = cfg.value("Multiplexing/Active", "yes").toString();
    text += tr("Multiplexing") + ": " + multiplexing;
    chkMux->setChecked(multiplexing != "no");

    label->setText(text);

    updatingModem = false;
    QTimer::singleShot(1000, this, SLOT(updateModem()));
}

static void appendValue(QString desc, QString file, QString *text)
{
    text->append(desc);
    text->append(": ");

    QFile f(file);
    if(!f.open(QFile::ReadOnly))
    {
        text->append("failed to open " + file + " " + f.errorString() + "\n");
    }
    else
    {
        text->append(f.readAll());
        f.close();
    }
}

void NeoControl::updateSysfs()
{
    if(screen != ScreenSysfs)
    {
        return;
    }

    QString text;
    appendValue(tr("Battery status"), "/sys/class/power_supply/battery/status", &text);
    appendValue(tr("Current"), "/sys/devices/platform/s3c24xx_pwm.0/hdq.0/bq27000-battery.0/power_supply/battery/current_now", &text);
    appendValue(tr("Modem power"), "/sys/bus/platform/devices/neo1973-pm-gsm.0/power_on", &text);
    appendValue(tr("GPS power"), "/sys/bus/platform/devices/neo1973-pm-gps.0/power_on", &text);
    appendValue(tr("Bluetooth power"), "/sys/bus/platform/devices/neo1973-pm-bt.0/power_on", &text);
    label->setText(text);

    QTimer::singleShot(1000, this, SLOT(updateSysfs()));
}
