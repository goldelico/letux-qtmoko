/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2009 Trolltech ASA.
** Copyright (C) 2011 qtmoko project
**
** Contact: Qt Extended Information (info@qtextended.org)
**
** This file may be used under the terms of the GNU General Public License
** version 2.0 as published by the Free Software Foundation and appearing
** in the file LICENSE.GPL included in the packaging of this file.
**
** Please review the following information to ensure GNU General Public
** Licensing requirements will be met:
**     http://www.fsf.org/licensing/licenses/info/GPLv2.html.
**
**
****************************************************************************/

#ifndef ROTATION_H
#define ROTATION_H

#include <QDialog>
//#include "ui_rotationsettingsbase.h"

class RotationSettings : public QDialog
//, private Ui::RotationSettingsBase
{
    Q_OBJECT

public:
    RotationSettings( QWidget* parent = 0, Qt::WFlags fl = 0 );
    ~RotationSettings();
	void setnewRotation(int r);

private:
    int changeRotTo;
};

#endif
