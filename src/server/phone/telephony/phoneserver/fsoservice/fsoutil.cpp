#include "fsoutil.h"

FsoUtil::FsoUtil(QObject *parent) : QObject(parent)
        , pendingReply()
        , pendingNotified(true)
        , pendingReceiver(NULL)
        , checkInterval(10)
{
}

FsoUtil::~FsoUtil()
{
}

FsoUtil FsoUtil::instance;

void FsoUtil::pendingCheck()
{
    if(!pendingReply.isFinished())
    {
        checkInterval += 10;
        QTimer::singleShot(checkInterval, this, SLOT(pendingCheck()));
    }
    else
    {
        pendingNotified = true;
        emit finished(pendingReply);
    }
}
