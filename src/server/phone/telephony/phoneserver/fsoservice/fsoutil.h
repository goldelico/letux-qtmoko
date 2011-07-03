#ifndef FSOUTIL_H
#define FSOUTIL_H

#include <QDebug>
#include <QtDBus>
#include <QTimer>
#include <qtelephonynamespace.h>
#include <qfsoutil.h>

template <class T, class T2, class T3>
        QTelephony::Result checkResult(QFsoDBusPendingReply<T, T2, T3> & reply,
                                       bool waitForFinished = true)
{
    if(checkReply(reply, waitForFinished))
    {
        return QTelephony::OK;
    }
    // TODO: convert FSO exception->QTelephony::Result
    return QTelephony::Error;
}

#endif // FSOUTIL_H
