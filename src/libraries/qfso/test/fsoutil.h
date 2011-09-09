#ifndef FSOUTIL_H
#define FSOUTIL_H

#include <QDebug>
#include <QtDBus>
#include <QTimer>
#include <qtelephonynamespace.h>
#include <qfsoutil.h>

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
        QTelephony::Result qTelResult(QFsoDBusPendingReply<T1, T2, T3, T4, T5, T6, T7, T8> & reply,
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
