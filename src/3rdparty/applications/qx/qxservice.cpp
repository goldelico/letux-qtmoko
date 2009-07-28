#include "qx.h"
#include "qxservice.h"
#include <stdio.h>
#include <QFile>

/*!
    \service RadioService Radio
    \brief Provides the Qtopia Radio service.

    The \i Radio service enables applications to control the radio
    tuner program.
*/

QxService::QxService( QObject *parent )
        : QtopiaAbstractService("QX", parent)
{
    publishAll();
    qx = (QX *) parent;
}

/*!
    Start X windows application.
*/
void QxService::startApp(QString name)
{
}

/*!
    Show the GUI for switching between running applications.
*/
void QxService::appSwitch()
{
    qx->pauseApp();
}
