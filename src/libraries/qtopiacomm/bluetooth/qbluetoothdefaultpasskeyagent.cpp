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

#include "qbluetoothdefaultpasskeyagent.h"

#include <QtopiaApplication>
#include <QString>
#include <QTimer>

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
    Constructs a new QBluetoothDefaultPasskeyAgent class.  The QObject parent is given
    by \a parent.
 */
QBluetoothDefaultPasskeyAgent::QBluetoothDefaultPasskeyAgent(const QString &name, QObject *parent)
    : QBluetoothPasskeyAgent(name, parent),
      m_passDialog(new QPasswordDialog)
{
    m_passDialog->setInputMode(QPasswordDialog::Plain);
}

/*!
    Destructor.
 */
QBluetoothDefaultPasskeyAgent::~QBluetoothDefaultPasskeyAgent()
{
    delete m_passDialog;
}

/*!
    \reimp
 */
void QBluetoothDefaultPasskeyAgent::requestPasskey(QBluetoothPasskeyRequest &req)
{
    QBluetoothLocalDevice device(req.localDevice());
    QBluetoothRemoteDevice remote(req.remoteDevice());
    device.updateRemoteDevice(remote);

    if (m_passDialog->isVisible()) {
        qLog(Bluetooth) << "Got new pairing request for"
            << remote.address().toString()
            << "but another pairing already in progress, rejecting new pair request";
        req.setRejected();
        return;
    }

    QString msg = "<P>";
    msg += tr("You are trying to pair with %1 [%2]. Please enter a PIN. You will need to enter the same PIN on the other device.", "%1=name, %2=address")
            .arg(remote.name()).arg(remote.address().toString());

    // brighten the screen
    QtopiaServiceRequest e1("QtopiaPowerManager", "setBacklight(int)");
    e1 << -1; // read brightness setting from config
    e1.send();

    // vibrate the phone if profile allows
    QPhoneProfileManager manager;
    QPhoneProfile profile = manager.activeProfile();
    if (profile.vibrate()) {
        QVibrateAccessory acc;
        acc.setVibrateNow(true);
        QTimer::singleShot(1000, this, SLOT(stopVibration()));
    }

    m_passDialog->reset();
    m_passDialog->setPrompt(msg);
    m_passDialog->setWindowModality(Qt::WindowModal);
    QtopiaApplication::execDialog(m_passDialog);

    QString pairPin = m_passDialog->password();
    if (!pairPin.isEmpty())
        req.setPasskey(pairPin);
    else
        req.setRejected();
}

/*!
    \reimp
 */
void QBluetoothDefaultPasskeyAgent::cancelRequest(const QString & /*localDevice*/, const QBluetoothAddress & /*remoteAddr*/)
{
    qLog(Bluetooth) << "QBluetoothDefaultPasskeyAgent::cancelRequest()";
    m_passDialog->reject();
}

/*!
    \reimp
 */
void QBluetoothDefaultPasskeyAgent::release()
{

}

/*!
    \internal
*/
void QBluetoothDefaultPasskeyAgent::stopVibration()
{
    QVibrateAccessory acc;
    acc.setVibrateNow(false);
}
