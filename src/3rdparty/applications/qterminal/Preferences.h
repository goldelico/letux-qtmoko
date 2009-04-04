
#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>

#include "profile.h"

class QAction;
class QLabel;

class Preferences : public QDialog
{
  Q_OBJECT

  public: 
    Preferences(QWidget *parent = 0, Qt::WindowFlags f = 0);
    Preferences(Profile *profile);
    ~Preferences();

    void setCurrentProfile(Profile *p);

    Profile *profile();
  //public signals:
    //void currentChanged();
  
  private slots:
    //void apply();
    void setFontName(QString);
    void setFontSize(int);
    void setCommand(const QString&);
    void setCursorShape(int);
    void setColorScheme(QString);
    void setHistoryLines(int);

  private:
    QAction *mCancel; 
    QLabel *preview;
    Profile *currentProfile;
    void setupUI();
    void updatePreview();
};

#endif
