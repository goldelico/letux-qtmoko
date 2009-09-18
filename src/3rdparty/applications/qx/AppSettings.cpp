#include "AppSettings.h"
#include <Qtopia>
#include <QDebug>


ProfileManager::ProfileManager()
{
    profConf = new QSettings(Qtopia::qtopiaDir() + "/etc/qx/profiles.conf", QSettings::IniFormat);
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
    prof.gps = profConf->value("gps").toBool();
    prof.rotate = profConf->value("rotate").toBool();
    profConf->endGroup();
    return prof;
}

void ProfileManager::SaveAppProfile(QString app, AppProfile prof)
{
    profConf->beginGroup(app);
    profConf->setValue("init", prof.init);
    profConf->setValue("antisuspend", prof.antiSuspend);
    profConf->setValue("antidim", prof.antiDim);
    profConf->setValue("gps", prof.gps);
    profConf->setValue("rotate", prof.rotate);
    profConf->endGroup();
}

//===========================================

SettingsDialog::SettingsDialog(QString appname, QWidget *parent) : QDialog(parent)
{
    app = appname;

    setFixedSize(380,350);
    setModal(true);
    setWindowTitle(tr("App Settings"));

    edt_init = new QLineEdit(this);
    chb_antisuspend = new QCheckBox(this);
    chb_antidim = new QCheckBox(this);
    chb_gps = new QCheckBox(this);
    chb_rotate = new QCheckBox(this);
    btn_save = new QPushButton(this);

    QLabel *initLabel = new QLabel(this);
    initLabel->setText(tr("Initialization script"));

    chb_antisuspend->setText(tr("Prevent suspending"));
    chb_antidim->setText(tr("Prevent screen dimming"));
    chb_gps->setText(tr("Enable GPS module"));
    chb_rotate->setText(tr("Rotate screen"));

    btn_save->setMinimumWidth(150);
    btn_save->setText(tr("Save"));

    QGridLayout *grid=new QGridLayout(this);
    grid->addWidget(initLabel,0,0,Qt::AlignCenter);
    grid->addWidget(edt_init,1,0);
    grid->addWidget(chb_antisuspend,2,0,Qt::AlignCenter);
    grid->addWidget(chb_antidim,3,0,Qt::AlignCenter);
    grid->addWidget(chb_gps,4,0,Qt::AlignCenter);
    grid->addWidget(chb_rotate,5,0,Qt::AlignCenter);
    grid->addWidget(btn_save,6,0,Qt::AlignCenter);

    connect (btn_save,SIGNAL(clicked()),this,SLOT(save()));

    //----

    mngr = new ProfileManager();

    AppProfile prof = mngr->GetAppProfile(app);
    edt_init->setText(prof.init);
    chb_antisuspend->setChecked(prof.antiSuspend);
    chb_antidim->setChecked(prof.antiDim);
    chb_gps->setChecked(prof.gps);
    chb_rotate->setChecked(prof.rotate);
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
    prof.gps = chb_gps->isChecked();
    prof.rotate = chb_rotate->isChecked();

    mngr->SaveAppProfile(app, prof);

    close();
}
