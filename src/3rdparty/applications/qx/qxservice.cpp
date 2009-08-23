#include "qx.h"
#include "qxservice.h"
#include <stdio.h>
#include <QFile>

/*!
    \service QX service
    \brief Provides service for controlling running X applications.

    The \i QX service enables user to pause, resume or stop X application.
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
