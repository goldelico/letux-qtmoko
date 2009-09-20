#ifndef APP_INFO_H
#define APP_INFO_H

#include <QDialog>
#include "DesktopScanner.h"

class InfoDialog : public QDialog
{
    Q_OBJECT
    public:
        InfoDialog(DesktopEntry entry, QWidget *);
};

#endif //APP_INFO_H
