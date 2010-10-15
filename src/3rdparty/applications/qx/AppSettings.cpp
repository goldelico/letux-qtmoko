#include "AppSettings.h"
#include <QDebug>


ProfileManager::ProfileManager()
{
#ifdef QTOPIA
    QString dir = Qtopia::qtopiaDir();
#else
    QString dir;
#endif
    profConf = new QSettings(dir + "/etc/qx/profiles.conf", QSettings::IniFormat);
}

ProfileManager::~ProfileManager()
{
    delete profConf;
}

AppProfile ProfileManager::GetAppProfile(QString app)
{
    AppProfile prof;
    profConf->beginGroup(app);
    prof.init = profConf->value("init").toString();
    prof.antiSuspend = profConf->value("antisuspend").toBool();
    prof.antiDim = profConf->value("antidim").toBool();
    prof.wm = profConf->value("wm").toBool();
    prof.kbd = profConf->value("kbd").toBool();
    prof.qvga = profConf->value("qvga").toBool();
    prof.rotate = profConf->value("rotate").toBool();
    prof.gps = profConf->value("gps").toBool();
    prof.matchbox = profConf->value("matchbox").toBool();
    prof.fullscreen = profConf->value("fullscreen").toBool();
    profConf->endGroup();
    return prof;
}

void ProfileManager::SaveAppProfile(QString app, AppProfile prof)
{
    profConf->beginGroup(app);
    profConf->setValue("init", prof.init);
    profConf->setValue("antisuspend", prof.antiSuspend);
    profConf->setValue("antidim", prof.antiDim);
    profConf->setValue("wm", prof.wm);
    profConf->setValue("kbd", prof.kbd);
    profConf->setValue("qvga", prof.qvga);
    profConf->setValue("rotate", prof.rotate);
    profConf->setValue("gps", prof.gps);
    profConf->setValue("matchbox", prof.matchbox);
    profConf->setValue("fullscreen", prof.fullscreen);
    profConf->endGroup();
}

//===========================================

SettingsDialog::SettingsDialog(QString appname, QWidget *parent) : QDialog(parent)
{
    app = appname;

    setFixedSize(380,420);
    setModal(true);
    setWindowTitle(tr("App Settings"));

    edt_init = new QLineEdit(this);
    chb_antisuspend = new QCheckBox(this);
    chb_antidim = new QCheckBox(this);
    chb_wm = new QCheckBox(this);
    chb_kbd = new QCheckBox(this);
    chb_qvga = new QCheckBox(this);
    chb_rotate = new QCheckBox(this);
    chb_gps = new QCheckBox(this);
    chb_matchbox = new QCheckBox(this);
    chb_fullscreen = new QCheckBox(this);
    btn_save = new QPushButton(this);

    QLabel *initLabel = new QLabel(this);
    initLabel->setText(tr("Initialization script"));

    chb_antisuspend->setText(tr("Prevent suspending"));
    chb_antidim->setText(tr("Prevent screen dimming"));
    chb_wm->setText(tr("Use window manager"));
    chb_kbd->setText(tr("Use virtual keyboard"));
    chb_qvga->setText(tr("Run in QVGA mode"));
    chb_rotate->setText(tr("Rotate screen"));
    chb_gps->setText(tr("Enable GPS module"));
    chb_matchbox->setText(tr("Use Matchbox"));
    chb_fullscreen->setText(tr("Fullscreen"));

    btn_save->setMinimumWidth(150);
    btn_save->setText(tr("Save"));

    QGridLayout *grid=new QGridLayout(this);
    grid->addWidget(initLabel,0,0,Qt::AlignCenter);
    grid->addWidget(edt_init,1,0);
    grid->addWidget(chb_antisuspend,2,0,Qt::AlignCenter);
    grid->addWidget(chb_antidim,3,0,Qt::AlignCenter);
    grid->addWidget(chb_wm,4,0,Qt::AlignCenter);
    grid->addWidget(chb_kbd,5,0,Qt::AlignCenter);
    grid->addWidget(chb_qvga,6,0,Qt::AlignCenter);
    grid->addWidget(chb_rotate,7,0,Qt::AlignCenter);
    grid->addWidget(chb_gps,8,0,Qt::AlignCenter);
    grid->addWidget(chb_matchbox,9,0,Qt::AlignCenter);
    grid->addWidget(chb_fullscreen,10,0,Qt::AlignCenter);
    grid->addWidget(btn_save,11,0,Qt::AlignCenter);

    connect (btn_save,SIGNAL(clicked()),this,SLOT(save()));

    //----

    mngr = new ProfileManager();

    AppProfile prof = mngr->GetAppProfile(app);
    edt_init->setText(prof.init);
    chb_antisuspend->setChecked(prof.antiSuspend);
    chb_antidim->setChecked(prof.antiDim);
    chb_wm->setChecked(prof.wm);
    chb_kbd->setChecked(prof.kbd);
    chb_qvga->setChecked(prof.qvga);
    chb_rotate->setChecked(prof.rotate);
    chb_gps->setChecked(prof.gps);
    chb_matchbox->setChecked(prof.matchbox);
    chb_fullscreen->setChecked(prof.fullscreen);
}

SettingsDialog::~SettingsDialog()
{
    delete mngr;
}

void SettingsDialog::save()
{
    AppProfile prof;
    prof.init = edt_init->text();
    prof.antiSuspend = chb_antisuspend->isChecked();
    prof.antiDim = chb_antidim->isChecked();
    prof.wm = chb_wm->isChecked();
    prof.kbd = chb_kbd->isChecked();
    prof.qvga = chb_qvga->isChecked();
    prof.rotate = chb_rotate->isChecked();
    prof.gps = chb_gps->isChecked();
    prof.matchbox = chb_matchbox->isChecked();
    prof.fullscreen = chb_fullscreen->isChecked();

    mngr->SaveAppProfile(app, prof);

    close();
}
