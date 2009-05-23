// -*- c++ -*-

#ifndef KVNCOPTIONSDIALOG_H
#define KVNCOPTIONSDIALOG_H

#include "ui_vncoptionsbase.h"

class KRFBOptions;

class KVNCOptionsDlg : public QDialog, Ui::VncOptionsBase
{
Q_OBJECT

public:
  KVNCOptionsDlg( KRFBOptions *options);
  ~KVNCOptionsDlg();

protected:
  void accept();

private:
  KRFBOptions *options;
};
#endif // KVNCOPTIONSDIALOG_H




