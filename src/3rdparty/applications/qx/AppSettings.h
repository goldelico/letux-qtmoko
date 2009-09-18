#ifndef APP_SETTINGS_H
#define APP_SETTINGS_H

#include <QSettings>

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>

struct AppProfile
{
    QString init;
    bool antiSuspend;
    bool antiDim;
    bool gps;
    bool rotate;
};

class ProfileManager
{
    public:
        ProfileManager();
        ~ProfileManager();
        AppProfile GetAppProfile(QString app);
        void SaveAppProfile(QString app, AppProfile prof);
    private:
        QSettings *profConf;
};

class SettingsDialog : public QDialog
{
    Q_OBJECT
    private:
        QLineEdit *edt_init;
        QCheckBox *chb_antisuspend;
        QCheckBox *chb_antidim;
        QCheckBox *chb_gps;
        QCheckBox *chb_rotate;
        QPushButton *btn_save;
        ProfileManager *mngr;
        QString app;
    public:
        SettingsDialog(QString appname, QWidget *);
        ~SettingsDialog();
    private slots:
        void save();
};

#endif //APP_SETTINGS_H
