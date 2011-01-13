/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2009 Trolltech ASA.
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

#include "btpinhelper.h"
#include "qtopiaserverapplication.h"

#include <QtopiaApplication>
#include <QString>
#include <QByteArray>
#include <QTextStream>
#include <QSettings>
#include <QTimer>
#include <QValueSpaceItem>

#include <qtopiaservices.h>
#include <qphoneprofile.h>
#include <qpassworddialog.h>
#include <qtopiaipcenvelope.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothlocaldevicemanager.h>
#include <qbluetoothpasskeyrequest.h>
#include <qbluetoothremotedevice.h>
#include <qtopialog.h>
#include <qvibrateaccessory.h>

/*!
    \class BluetoothPasskeyAgentTask
    \inpublicgroup QtBluetoothModule
    \ingroup QtopiaServer::Task::Bluetooth
    \brief The BluetoothPasskeyAgentTask class provides a default passkey agent for performing Bluetooth passkey authentications.

    The BluetoothPasskeyAgentTask class implements a Qt Extended global passkey agent.  The
    internal implementation uses the QPasswordDialog to ask the user for
    the passkey.

    This class is part of the Qt Extended server and cannot be used by other QtopiaApplications.
    \sa QBluetoothPasskeyAgent
 */

/*!
    Constructs a new BluetoothPasskeyAgentTask class.  The QObject parent is given
    by \a parent.
 */
BluetoothPasskeyAgentTask::BluetoothPasskeyAgentTask(QObject* parent)
    : QObject( parent )
{
    //we start this once the GUI is up and running
    serverWidgetVsi = new QValueSpaceItem("/System/ServerWidgets/Initialized", this);
    connect( serverWidgetVsi, SIGNAL(contentsChanged()), this, SLOT(delayedAgentStart()) );
    delayedAgentStart(); //in case its visible already
}

/*!
  \internal
  */
void BluetoothPasskeyAgentTask::delayedAgentStart()
{
    if ( serverWidgetVsi && serverWidgetVsi->value( QByteArray(), false ).toBool() ) {
        serverWidgetVsi->disconnect();
        serverWidgetVsi->deleteLater();
        serverWidgetVsi = 0;
        QTimer::singleShot( 5000, this, SLOT(activateAgent()) );
    }
}

/*!
  \internal
  */
void BluetoothPasskeyAgentTask::activateAgent()
{
    QBluetoothDefaultPasskeyAgent *agent = new QBluetoothDefaultPasskeyAgent("DefaultPasskeyAgent", this);
    
    QBluetoothLocalDeviceManager manager;
    QBluetoothLocalDevice defaultDevice(manager.defaultDevice());
    qLog(Bluetooth) << "Registering BTPinHelper for device " << defaultDevice.adapterPath();
    defaultDevice.registerAgent(agent);
}

QTOPIA_TASK(DefaultBluetoothPassKeyAgent, BluetoothPasskeyAgentTask);
