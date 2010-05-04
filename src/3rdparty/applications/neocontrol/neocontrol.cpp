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
    case ScreenSysfs:
        showScreen(ScreenInit);
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
    label->setVisible(scr == ScreenInit || scr == ScreenSysfs);
    bBack->setText(scr == ScreenInit ? tr("Quit") : tr("Back"));
    lineEdit->setVisible(false);

    switch(scr)
    {
    case ScreenInit:
        label->setText(tr("Neo hardware tool"));
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
    appendValue(tr("Current"), "/sys/class/power_supply/battery/current_now", &text);
    appendValue(tr("Modem power"), "/sys/devices/platform/s3c2440-i2c/i2c-0/0-0073/pcf50633-gpio/reg-fixed-voltage.1/gta02-pm-gsm.0/power_on", &text);
    appendValue(tr("GPS power"), "/sys/devices/platform/gta02-pm-gps.0/power_on", &text);
    appendValue(tr("Bluetooth power"), "/sys/devices/platform/gta02-pm-bt.0/power_on", &text);
    label->setText(text);

    QTimer::singleShot(1000, this, SLOT(updateSysfs()));
}
