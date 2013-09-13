#include "neocontrol.h"
#include <QPainter>

NeoControl::NeoControl(QWidget * parent, Qt::WFlags f)
:  QWidget(parent)
#ifdef QTOPIA
    , chargeLogVsi("/UI/Battery/charge_log")
#endif
{
#ifdef QTOPIA
    this->setWindowState(Qt::WindowMaximized);
#else
    Q_UNUSED(f);
#endif
    bBack = new QPushButton(this);
    connect(bBack, SIGNAL(clicked()), this, SLOT(backClicked()));

    bNext = new QPushButton(tr("Next"), this);
    connect(bNext, SIGNAL(clicked()), this, SLOT(nextClicked()));

    bSave = new QPushButton(tr("Save"), this);
    connect(bSave, SIGNAL(clicked()), this, SLOT(saveClicked()));

    chkMux = new QCheckBox(tr("Multiplexing"), this);
    connect(chkMux, SIGNAL(stateChanged(int)), this,
            SLOT(muxStateChanged(int)));

    chkFso = new QCheckBox(tr("Use FSO (freesmartphone.org)"), this);
    connect(chkFso, SIGNAL(stateChanged(int)), this,
            SLOT(fsoStateChanged(int)));

    label = new QLabel(this);
    normalFont = label->font();
    smallFont = QFont(normalFont.family(), (3 * normalFont.pointSize()) / 5);
    lineEdit = new QLineEdit(this);

    label4 = new QLabel(this);
    label5 = new QLabel(this);

    slider4 = new MixerSlider(this);
    slider5 = new MixerSlider(this);

    buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignBottom);
    buttonLayout->addWidget(bBack);
    buttonLayout->addWidget(bNext);

    layout = new QVBoxLayout(this);
    layout->addWidget(label);
    layout->addWidget(label4);
    layout->addWidget(slider4);
    layout->addWidget(label5);
    layout->addWidget(slider5);
    layout->addWidget(bSave);
    layout->addWidget(lineEdit);
    layout->addWidget(chkMux);
    layout->addWidget(chkFso);
    layout->addLayout(buttonLayout);

    showScreen(NeoControl::ScreenCharge);
}

NeoControl::~NeoControl()
{

}

// Qtopia compat for Qt only PC version
#ifndef QTOPIA
namespace Qtopia
{
    QByteArray readFile(const char *path);
    QDateTime rtcNow();
}
QByteArray Qtopia::readFile(const char *path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning() << "readFile failed" << path << ":" << f.errorString();
        return QByteArray();
    }
    QByteArray content = f.readAll();
    f.close();
    return content;
}

QDateTime Qtopia::rtcNow()
{
    QByteArray secsStr = readFile("/sys/class/rtc/rtc0/since_epoch").trimmed();
    uint secs = secsStr.toUInt();
    return QDateTime::fromTime_t(secs);
}
#endif

void NeoControl::backClicked()
{
    switch (screen) {
    case ScreenInit:
        close();
        break;
    case ScreenRtc:
        showScreen(ScreenInit);
        break;
    case ScreenMixer:
        showScreen(ScreenRtc);
        break;
    case ScreenModem:
        showScreen(ScreenMixer);
        break;
    case ScreenSysfs:
        showScreen(ScreenModem);
        break;
    case ScreenCharge:
        showScreen(ScreenSysfs);
        break;
    }
}

void NeoControl::nextClicked()
{
    switch (screen) {
    case ScreenInit:
        showScreen(ScreenRtc);
        break;
    case ScreenRtc:
        showScreen(ScreenMixer);
        break;
    case ScreenMixer:
        showScreen(ScreenModem);
        break;
    case ScreenModem:
        showScreen(ScreenSysfs);
        break;
    case ScreenSysfs:
        showScreen(ScreenCharge);
        break;
    case ScreenCharge:
        break;
    }
}

void NeoControl::saveClicked()
{
    if (screen == ScreenMixer) {
        system
            ("alsactl -f /opt/qtmoko/etc/alsa/gta04_initial_alsa.state store");
    }
}

void NeoControl::showScreen(NeoControl::Screen scr)
{
    if (scr == ScreenMixer) {
        openAlsaMixer();
    }
    if (this->screen == ScreenMixer) {
        closeAlsaMixer();
    }
    if (this->screen == ScreenSysfs) {
        label->setFont(normalFont);
    }
    if (scr == ScreenSysfs) {
        label->setFont(smallFont);
    }
    if (this->screen == ScreenCharge) {
        setFont(normalFont);
    }
    if (scr == ScreenCharge) {
        setFont(smallFont);
    }

    this->screen = scr;

    label->setVisible(scr == ScreenInit || scr == ScreenRtc
                      || scr == ScreenMixer || scr == ScreenModem
                      || scr == ScreenSysfs);
    bBack->setText(scr == ScreenInit ? tr("Quit") : tr("Back"));
    lineEdit->setVisible(false);
    chkMux->setVisible(scr == ScreenModem);
    chkFso->setVisible(scr == ScreenModem);
    label4->setVisible(scr == ScreenMixer);
    label5->setVisible(scr == ScreenMixer);
    slider4->setVisible(scr == ScreenMixer);
    slider5->setVisible(scr == ScreenMixer);
    bSave->setVisible(scr == ScreenMixer);

    switch (scr) {
    case ScreenInit:
        label->setText(tr("Neo hardware tool"));
        break;
    case ScreenRtc:
        updateRtc();
        break;
    case ScreenMixer:
        updateMixer();
        break;
    case ScreenModem:
        updateModem();
        break;
    case ScreenCharge:
        updateCharge();
        break;
    case ScreenSysfs:
        updateSysfs();
        break;

    default:
        break;
    }
}

static int computeCurrent(int secs, int chargeBefore, int chargeAfter)
{
    if (secs == 0)
        return 0;
    return ((chargeBefore - chargeAfter) * 36) / (10 * secs);
}

void NeoControl::paintEvent(QPaintEvent *)
{
    if (screen != ScreenCharge) {
        return;
    }

    QList < QString > lines = chargeLog.split('\n');
    if (lines.count() < 2)
        return;

    QList < QDateTime > dates;
    QList < int >charges;

    QDateTime dtMin(QDate(2999, 1, 1));
    QDateTime dtMax(QDate(1899, 1, 1));
    int chargeMax = 0;

    for (int i = 0; i < lines.count(); i++) {
        QList < QString > values = lines.at(i).split('\t');
        if (values.count() < 2) {
            continue;
        }
        QDateTime dt =
            QDateTime::fromString(values.at(0), "yyyy-MM-dd hh:mm:ss");
        int charge = values.at(1).toInt();
        dates.append(dt);
        charges.append(charge);

        dtMin = (dtMin < dt ? dtMin : dt);
        dtMax = (dtMax > dt ? dtMax : dt);
        chargeMax = (chargeMax > charge ? chargeMax : charge);
    }

    int w = (9 * this->width()) / 10;
    int h = bNext->y() - bNext->height();

    int totalSecs = dtMin.secsTo(dtMax);

    if (chargeMax == 0 || totalSecs == 0)
        return;

    QPainter p(this);

    int fontW = p.fontMetrics().width('w');
    int fontH = p.fontMetrics().height();

    p.translate(fontW, fontH);

    p.drawLine(0, h, w, h);
    p.drawLine(0, 0, 0, h);

    QPen pen = p.pen();

    for (int round = 0; round <= 1; round++) {
        int chargeY = 0x7fffffff;
        int currentX = 0x7fffffff;
        int currentY = 0x7fffffff;
        int hourTextX = 0x7fffffff;

        int x1 = -1;
        int y1 = -1;
        int prevSecs = -1;
        int prevCharge = -1;

        for (int i = 0; i < dates.count(); i++) {
            QDateTime dt = dates.at(i);
            int charge = charges.at(i);
            int secs = dtMin.secsTo(dt);

            int x2 = (w * secs) / totalSecs;
            int y2 = h - (h * charge) / chargeMax;

            // Draw time on x axis
            if (abs(x2 - hourTextX) > 5 * fontW) {
                if (round == 0) {
                    p.drawText(x2, h + fontH, dt.toString("hh:mm"));
                    p.setPen(Qt::darkGreen);
                    p.drawLine(x2, 0, x2, h);
                    p.setPen(pen);
                }
                hourTextX = x2;
            }
            // Draw charge point and charge value
            p.drawEllipse(x2 - 2, y2 - 2, 4, 4);
            int y = y2 + fontH / 2;
            if (abs(y2 - chargeY) > 2 * fontH) {
                QString text = QString::number(charge);
                p.drawText(x2 + fontW, y, text);
                chargeY = y;
            }
            // Draw charge line and in the middle write current
            if (x1 >= 0) {
                p.drawLine(x1, y1, x2, y2);
                y = (y1 + y2) / 2;
                int x = (x1 + x2) / 2;
                int shiftY = (y > h / 2 ? -h : h) / 4;
                int fontShiftY = (y > h / 2 ? -fontH : fontH);
                if (abs(y - currentY) > 2 * fontH
                    || abs(x - currentX) > 5 * fontW) {
                    int current =
                        computeCurrent(secs - prevSecs, prevCharge, charge);
                    if (round == 0) {
                        p.setPen(Qt::darkYellow);
                        p.drawLine(x, y, x, y + shiftY - fontShiftY);
                        p.setPen(pen);
                    }
                    QString text = QString::number(current) + "mA";
                    int textW = p.fontMetrics().width(text);
                    p.fillRect(x, y + shiftY - fontShiftY, textW, fontH,
                               Qt::yellow);
                    p.drawText(x, y + shiftY, text);
                    currentY = y;
                    currentX = x;
                }
            }

            x1 = x2;
            y1 = y2;
            prevSecs = secs;
            prevCharge = charge;
        }
    }
}

void NeoControl::updateRtc()
{
    if (screen != ScreenRtc)
        return;

    QDateTime rtcNow = Qtopia::rtcNow();
    QString rtcDate = Qtopia::readFile("/sys/class/rtc/rtc0/date").trimmed();
    QString rtcTime = Qtopia::readFile("/sys/class/rtc/rtc0/time").trimmed();

    QByteArray wakealarmStr =
        Qtopia::readFile("/sys/class/rtc/rtc0/wakealarm").trimmed();
    QString alarmStr;
    if (wakealarmStr.isEmpty()) {
        alarmStr = tr("not set");
    } else {
        uint wakealarmSecs = wakealarmStr.toUInt();
        QDateTime wakealarmDt = QDateTime::fromTime_t(wakealarmSecs);
        alarmStr = wakealarmDt.toString();
    }

    label->
        setText(QString
                (tr
                 ("RTC (Real time clock)\n\nDate: %1\nTime: %2\nLocal: %3\nAlarm: %4"))
                .arg(rtcDate)
                .arg(rtcTime)
                .arg(rtcNow.toString())
                .arg(alarmStr));

    QTimer::singleShot(1000, this, SLOT(updateRtc()));
}

int NeoControl::openAlsaMixer()
{
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

    system("alsactl -f /opt/qtmoko/etc/alsa/gta04_initial_alsa.state restore");
}

void NeoControl::updateMixer()
{
    if (screen != ScreenMixer) {
        return;
    }
    if (slider4->sliding || slider5->sliding) {
        QTimer::singleShot(100, this, SLOT(updateMixer()));
        return;
    }

    snd_mixer_elem_t *elem;
    snd_mixer_elem_t *elem4 = NULL;
    snd_mixer_elem_t *elem5 = NULL;

    for (elem = snd_mixer_first_elem(mixerFd); elem;
         elem = snd_mixer_elem_next(elem)) {
        QString elemName = QString(snd_mixer_selem_get_name(elem));

        if (elemName == "DAC2 Digital Fine") {
            elem4 = elem;
        } else if (elemName == "Analog") {
            elem5 = elem;
        }
    }

    slider4->setMixerElem(elem4, true);
    slider5->setMixerElem(elem5, false);

    label4->setText(tr("Playback %1").arg(slider4->volume));    // Mono Playback Volume
    label5->setText(tr("Microphone %1").arg(slider5->volume));  // Mono Sidetone Playback Volume

    label->setText(tr("Call volume settings"));

    QTimer::singleShot(1000, this, SLOT(updateMixer()));
}

void NeoControl::muxStateChanged(int state)
{
    if (updatingModem) {
        return;
    }
    QString val = (state == Qt::Checked ? "yes" : "no");
    QSettings cfg("Trolltech", "Modem");
    cfg.setValue("Multiplexing/Active", val);
    cfg.sync();

    QMessageBox::information(this, tr("Multiplexing"),
                             tr
                             ("Settings will be activated after restarting QtExtended with POWER button"));
}

QString NeoControl::getQpeEnv()
{
    QFile f("/opt/qtmoko/qpe.env");
    if (!f.open(QFile::ReadOnly)) {
        QMessageBox::critical(this, tr("FSO"),
                              tr("Failed to read") + " " + f.fileName());
        return "";
    }
    QString content = f.readAll();
    f.close();
    return content;
}

void NeoControl::setQpeEnv(bool fso)
{
    QString content = getQpeEnv();
    QString fsoStr = "export QTOPIA_PHONE=Fso";
    QString atStr = "export QTOPIA_PHONE=AT";
    if (fso) {
        content = content.replace(atStr, fsoStr);
    } else {
        content = content.replace(fsoStr, atStr);
    }
    QFile f("/opt/qtmoko/qpe.env");
    if (!f.open(QFile::WriteOnly)) {
        QMessageBox::critical(this, tr("FSO"),
                              tr("Failed to write to") + " " + f.fileName());
        return;
    }
    f.write(content.toLatin1());
    f.close();
    QMessageBox::information(this, tr("FSO"),
                             tr
                             ("You have to restart your phone for changes to take place"));
}

void NeoControl::fsoStateChanged(int)
{
    if (updatingModem) {
        return;
    }
    QTimer::singleShot(0, this, SLOT(fsoChange()));
}

void NeoControl::fsoChange()
{
    bool checked = chkFso->isChecked();
    if (!checked) {
        QProcess::execute("qterminal",
                          QStringList() << "-c" << "update-rc.d" << "-f" <<
                          "fso-deviced" << "remove");
        setQpeEnv(false);       // disable FSO
        return;
    }
    if (!QFile::exists("/usr/sbin/fsogsmd")) {
        QMessageBox::information(this, tr("FSO"),
                                 tr
                                 ("FSO packages have to be downloaded and installed. Please make sure you have internet connection now."));
        QProcess::execute("raptor",
                          QStringList() << "-u" << "-i" << "fso-gsmd-openmoko"
                          << "fso-usaged-openmoko");
        QMessageBox::information(this, tr("FSO"),
                                 tr
                                 ("QtMoko needs very recent FSO, it will be downloaded from http://activationrecord.net/radekp/pub/libfsogsm.so.0.0.0"));
        QProcess::execute("qterminal",
                          QStringList() << "-c" << "wget" <<
                          "http://activationrecord.net/radekp/pub/libfsogsm.so.0.0.0");
        QProcess::execute("qterminal",
                          QStringList() << "-c" << "mv" << "libfsogsm.so.0.0.0"
                          <<
                          "/usr/lib/cornucopia/libs/fsogsm/libfsogsm.so.0.0.0");
    }
    QProcess::execute("qterminal",
                      QStringList() << "-c" << "update-rc.d" << "fso-deviced" <<
                      "defaults");
    setQpeEnv(true);
}

void NeoControl::updateModem()
{
    if (screen != ScreenModem) {
        return;
    }
    updatingModem = true;

    QString text(tr("Modem settings\n\n"));
    QSettings cfg("Trolltech", "Modem");

    QString multiplexing = cfg.value("Multiplexing/Active", "yes").toString();
    text += tr("Multiplexing") + ": " + multiplexing;
    chkMux->setChecked(multiplexing != "no");

    label->setText(text);

    QString qpeEnv = getQpeEnv();
    QString fsoStr = "export QTOPIA_PHONE=Fso";
    chkFso->setChecked(qpeEnv.indexOf(fsoStr) >= 0);

    updatingModem = false;
    QTimer::singleShot(1000, this, SLOT(updateModem()));
}

static void appendValue(QString desc, QString file, QString * text,
                        QByteArray replaceBefore = "", QByteArray replaceAfter =
                        "")
{
    text->append(desc);
    text->append(": ");

    QFile f(file);
    if (!f.open(QFile::ReadOnly)) {
        text->append("failed to open " + file + " " + f.errorString() + "\n");
    } else {
        QByteArray content = f.readAll();
        if (content.length() == 0) {
            text->append('\n');
        } else {
            if (replaceBefore.count() > 0) {
                content = content.replace(replaceBefore, replaceAfter);
            }
            text->append(content);
        }
        f.close();
    }
}

void NeoControl::updateSysfs()
{
    if (screen != ScreenSysfs) {
        return;
    }

    QString text;
    appendValue(tr("Battery"), "/sys/class/power_supply/bq27000-battery/uevent",
                &text, "POWER_SUPPLY_", "  ");
    appendValue(tr("USB"), "/sys/class/power_supply/twl4030_usb/uevent", &text,
                "POWER_SUPPLY_", "  ");
    appendValue(tr("USB max current"),
                "/sys/bus/platform/drivers/twl4030_bci/twl4030_bci/max_current",
                &text);

    label->setText(text);

    QTimer::singleShot(1000, this, SLOT(updateSysfs()));
}

void NeoControl::updateCharge()
{
    if (screen != ScreenCharge) {
        return;
    }
#ifdef QTOPIA
    chargeLog = chargeLogVsi.value().toString();
#else
    chargeLog =
        "2013-09-12 23:07:47\t1002813\n2013-09-12 23:13:11\t1006740\n2013-09-12 23:18:11\t1009417\n2013-09-12 23:23:41\t1012273\n2013-09-12 23:28:41\t1014772\n2013-09-12 23:33:41\t1017450\n2013-09-12 23:38:41\t1020127\n2013-09-12 23:44:11\t1023162\n2013-09-12 23:49:11\t1025839\n2013-09-12 23:54:41\t1028695\n2013-09-12 23:59:41\t1025661\n2013-09-13 00:05:11\t1007275\n2013-09-13 00:10:41\t988711\n2013-09-13 00:15:41\t971932\n2013-09-13 00:20:41\t954975\n2013-09-13 00:25:42\t938196\n2013-09-13 00:30:42\t921238\n2013-09-13 00:36:11\t902853\n2013-09-13 00:41:12\t886074\n2013-09-13 00:46:41\t867331\n2013-09-13 00:51:42\t850731\n2013-09-13 00:57:11\t842877\n2013-09-13 01:02:11\t875185\n2013-09-13 01:07:12\t907494\n2013-09-13 01:12:41\t936411\n2013-09-13 01:17:42\t938553\n2013-09-13 01:23:11\t940516\n2013-09-13 01:28:11\t942123\n2013-09-13 01:33:11\t946228\n2013-09-13 01:38:41\t950155\n2013-09-13 01:43:41\t953547\n2013-09-13 01:49:11\t957474\n2013-09-13 01:54:11\t960865\n2013-09-13 01:59:41\t964792\n2013-09-13 02:04:41\t968362\n2013-09-13 02:09:41\t971932\n2013-09-13 02:15:11\t975859\n2013-09-13 02:20:41\t979608\n2013-09-13 02:26:11\t983356\n2013-09-13 06:22:12\t853944\n2013-09-13 06:37:48\t841270\n2013-09-13 07:03:52\t796288\n2013-09-13 07:08:52\t762552\n2013-09-13 07:13:52\t729172\n2013-09-13 07:18:52\t695079\n2013-09-13 07:24:22\t657772\n2013-09-13 07:29:52\t651882\n2013-09-13 07:34:52\t665091\n2013-09-13 07:40:22\t679549\n2013-09-13 07:45:22\t692937\n2013-09-13 07:50:22\t706146\n2013-09-13 07:55:22\t719533\n2013-09-13 08:00:52\t734170\n2013-09-13 08:05:52\t747558\n2013-09-13 08:10:52\t760945\n2013-09-13 08:16:22\t775582\n2013-09-13 08:21:22\t788791\n2013-09-13 08:26:22\t802000\n2013-09-13 08:31:22\t815209\n2013-09-13 08:36:22\t828418\n2013-09-13 08:41:52\t843055\n2013-09-13 08:47:22\t857692\n2013-09-13 08:52:22\t870901\n2013-09-13 08:57:22\t884289\n2013-09-13 09:02:52\t898926\n2013-09-13 09:07:52\t912135\n2013-09-13 09:12:52\t925165\n2013-09-13 09:17:52\t935518\n2013-09-13 09:22:52\t936411\n2013-09-13 09:27:53\t908386\n2013-09-13 09:33:22\t877506\n2013-09-13 09:38:22\t892500\n2013-09-13 09:43:52\t915526\n2013-09-13 09:49:22\t936054\n2013-09-13 09:54:52\t939088\n2013-09-13 09:59:52\t941766\n2013-09-13 10:05:22\t944800\n2013-09-13 10:10:22\t947656\n2013-09-13 10:15:22\t949977\n2013-09-13 10:20:52\t953725\n2013-09-13 10:26:22\t957831\n2013-09-13 10:31:22\t961579\n2013-09-13 10:36:52\t965506\n2013-09-13 10:42:22\t969612\n2013-09-13 10:47:22\t973360\n2013-09-13 10:52:22\t977109\n2013-09-13 10:57:52\t981036\n2013-09-13 11:02:52\t984606\n2013-09-13 11:07:52\t988533\n2013-09-13 11:12:52\t992281\n2013-09-13 11:18:22\t995316\n2013-09-13 11:23:52\t998350\n2013-09-13 11:28:52\t1001028\n2013-09-13 11:33:52\t1003705\n2013-09-13 11:38:52\t1006561\n2013-09-13 11:43:52\t1009239\n2013-09-13 11:48:52\t1012095\n2013-09-13 11:53:52\t1014772\n2013-09-13 11:58:52\t1017450\n2013-09-13 12:04:22\t1019770\n2013-09-13 12:09:22\t1022448\n2013-09-13 12:14:22\t1025482\n2013-09-13 12:19:22\t1028517\n2013-09-13 12:24:52\t1030837\n2013-09-13 12:30:22\t1030837\n2013-09-13 12:35:52\t1030837\n2013-09-13 12:41:22\t1030837\n2013-09-13 12:46:52\t1030837\n2013-09-13 12:51:52\t1030837\n2013-09-13 12:56:52\t1030837\n2013-09-13 13:01:52\t1030837\n2013-09-13 13:07:22\t1030837";
#endif

    QTimer::singleShot(10000, this, SLOT(updateCharge()));
    update();
}
