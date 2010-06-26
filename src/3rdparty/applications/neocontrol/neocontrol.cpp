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

    bSave = new QPushButton(tr("Save"), this);
    connect(bSave, SIGNAL(clicked()), this, SLOT(saveClicked()));

    chkDeepSleep = new QCheckBox(tr("Deep sleep"), this);
    connect(chkDeepSleep, SIGNAL(stateChanged(int)), this, SLOT(deepSleepStateChanged(int)));

    chkMux = new QCheckBox(tr("Multiplexing"), this);
    connect(chkMux, SIGNAL(stateChanged(int)), this, SLOT(muxStateChanged(int)));

    label = new QLabel(this);
    lineEdit = new QLineEdit(this);

    label5 = new QLabel(this);
    label12 = new QLabel(this);
    label48 = new QLabel(this);

    slider5 = new MixerSlider(this);
    slider12 = new MixerSlider(this);
    slider48 = new MixerSlider(this);

    buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignBottom);
    buttonLayout->addWidget(bBack);
    buttonLayout->addWidget(bNext);

    layout = new QVBoxLayout(this);
    layout->addWidget(bQvga);
    layout->addWidget(label);
    layout->addWidget(label5);
    layout->addWidget(slider5);
    layout->addWidget(label12);
    layout->addWidget(slider12);
    layout->addWidget(label48);
    layout->addWidget(slider48);
    layout->addWidget(bSave);
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
    case ScreenMixer:
        showScreen(ScreenInit);
        break;
    case ScreenModem:
        showScreen(ScreenMixer);
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
        showScreen(ScreenMixer);
        break;
    case ScreenMixer:
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

void NeoControl::saveClicked()
{
    if(screen == ScreenMixer)
    {
        system("alsactl -f /usr/share/openmoko/scenarios/gsmhandset.state store");
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
    if(scr == ScreenMixer)
    {
        openAlsaMixer();
    }
    if(this->screen == ScreenMixer)
    {
        closeAlsaMixer();
    }

    this->screen = scr;

    bQvga->setVisible(scr == ScreenDisplay);
    label->setVisible(scr == ScreenInit || scr == ScreenMixer || scr == ScreenModem || scr == ScreenSysfs);
    bBack->setText(scr == ScreenInit ? tr("Quit") : tr("Back"));
    lineEdit->setVisible(false);
    chkDeepSleep->setVisible(scr == ScreenModem);
    chkMux->setVisible(scr == ScreenModem);
    label5->setVisible(scr == ScreenMixer);
    label12->setVisible(scr == ScreenMixer);
    label48->setVisible(scr == ScreenMixer);
    slider5->setVisible(scr == ScreenMixer);
    slider12->setVisible(scr == ScreenMixer);
    slider48->setVisible(scr == ScreenMixer);
    bSave->setVisible(scr == ScreenMixer);

    switch(scr)
    {
    case ScreenInit:
        label->setText(tr("Neo hardware tool"));
        break;
    case ScreenMixer:
        updateMixer();
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

int NeoControl::openAlsaMixer()
{
    system("alsactl -f /usr/share/openmoko/scenarios/gsmhandset.state restore");

    int ret = 0;
    QString text(tr("Call volume settings\n\n"));

    if ((ret = snd_mixer_open(&mixerFd, 0)) < 0) {
        text += QString("snd_mixer_open error %1").arg(ret);
        goto err;
    }
    if ((ret = snd_mixer_attach(mixerFd, "default")) < 0) {
        text += QString("snd_mixer_attach error %1").arg(ret);
        goto err;
    }
    if ((ret = snd_mixer_selem_register(mixerFd, NULL, NULL)) < 0) {
        text += QString("snd_mixer_selem_register error %1").arg(ret);
        goto err;
    }
    if ((ret = snd_mixer_load(mixerFd)) < 0) {
        text += QString("snd_mixer_load error %1").arg(ret);
        goto err;
    }

    goto ok;

    err:
    if (mixerFd)
        snd_mixer_close(mixerFd);
    mixerFd = NULL;
    ok:
    label->setText(text);

    return ret;
}

void NeoControl::closeAlsaMixer()
{
    if (mixerFd) {
        snd_mixer_detach(mixerFd, "default");
        snd_mixer_close(mixerFd);
        mixerFd = NULL;
    }

    system("alsactl -f /usr/share/openmoko/scenarios/stereoout.state restore");
}

void NeoControl::updateMixer()
{
    if(screen != ScreenMixer)
    {
        return;
    }
    if(slider5->sliding || slider12->sliding || slider48->sliding)
    {
        QTimer::singleShot(100, this, SLOT(updateMixer()));
        return;
    }

    snd_mixer_elem_t *elem;
    snd_mixer_elem_t *elem5 = NULL;
    snd_mixer_elem_t *elem12 = NULL;
    snd_mixer_elem_t *elem48 = NULL;

    for (elem = snd_mixer_first_elem(mixerFd); elem;
    elem = snd_mixer_elem_next(elem)) {
        QString elemName = QString(snd_mixer_selem_get_name(elem));

        if(elemName == "Speaker")
        {
            elem5 = elem;
        }
        else if(elemName == "Mono Sidetone")
        {
            elem12 = elem;
        }
        else if(elemName == "Mic2")
        {
            elem48 = elem;
        }
    }

    slider5->setMixerElem(elem5, true);
    slider12->setMixerElem(elem12, true);
    slider48->setMixerElem(elem48, false);

    label5->setText(tr("Playback (control.5) %1").arg(slider5->volume));        // Mono Playback Volume
    label12->setText(tr("Sidetone (control.12) %1").arg(slider12->volume));     // Mono Sidetone Playback Volume
    label48->setText(tr("Mic2 (control.48) %1").arg(slider48->volume));         // Mic2 Capture Volume

    label->setText(tr("Call volume settings"));

    QTimer::singleShot(1000, this, SLOT(updateMixer()));
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
    appendValue(tr("Current"), "/sys/class/power_supply/battery/current_now", &text);
    appendValue(tr("Modem power"), "/sys/devices/platform/s3c2440-i2c/i2c-0/0-0073/pcf50633-gpio/reg-fixed-voltage.1/gta02-pm-gsm.0/power_on", &text);
    appendValue(tr("GPS power"), "/sys/devices/platform/gta02-pm-gps.0/power_on", &text);
    appendValue(tr("Bluetooth power"), "/sys/devices/platform/gta02-pm-bt.0/power_on", &text);
    label->setText(text);

    QTimer::singleShot(1000, this, SLOT(updateSysfs()));
}
