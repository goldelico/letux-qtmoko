/****************************************************************************
**
** QtMoko interprocess read/write lock for the shared-memory cache.
** Uses System V semaphores instead of Qt's private QLock API.
**
****************************************************************************/
#ifndef QTOPIASHAREDMEMORYLOCK_H
#define QTOPIASHAREDMEMORYLOCK_H

#include <QString>
#include <QByteArray>
#include <QDebug>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>
#include <string.h>

class QtopiaSharedMemoryLock
{
public:
    enum Type { Read, Write };
    enum { MaxReaders = 200 };

    QtopiaSharedMemoryLock(const QString &filename, char id, bool create = false)
        : semId(-1), count(0), type(Read), owner(create)
    {
        const QByteArray path = filename.toLocal8Bit();
        const key_t key = ftok(path.constData(), id);
        if (key == (key_t)-1) {
            qWarning("QtopiaSharedMemoryLock: ftok failed: %s", strerror(errno));
            return;
        }
        if (create) {
            int oldId = semget(key, 1, 0);
            if (oldId != -1)
                semctl(oldId, 0, IPC_RMID);
            semId = semget(key, 1, IPC_CREAT | IPC_EXCL | 0600);
            if (semId != -1 && semctl(semId, 0, SETVAL, MaxReaders) == -1) {
                semctl(semId, 0, IPC_RMID);
                semId = -1;
            }
        } else {
            semId = semget(key, 1, 0);
        }
        if (semId == -1)
            qWarning("QtopiaSharedMemoryLock: cannot %s semaphore: %s",
                     create ? "create" : "open", strerror(errno));
    }

    ~QtopiaSharedMemoryLock()
    {
        while (count > 0)
            unlock();
        if (owner && semId != -1)
            semctl(semId, 0, IPC_RMID);
    }

    bool isValid() const { return semId != -1; }
    bool locked() const { return count > 0; }

    void lock(Type requested)
    {
        if (!isValid())
            qFatal("QtopiaSharedMemoryLock: invalid semaphore");
        if (count) {
            if (type == Read && requested == Write)
                qFatal("QtopiaSharedMemoryLock: cannot upgrade read lock to write lock");
            ++count;
            return;
        }
        struct sembuf op;
        op.sem_num = 0;
        op.sem_op = requested == Write ? -MaxReaders : -1;
        op.sem_flg = SEM_UNDO;
        int result;
        do {
            result = semop(semId, &op, 1);
        } while (result == -1 && errno == EINTR);
        if (result == -1)
            qFatal("QtopiaSharedMemoryLock: lock failed: %s", strerror(errno));
        type = requested;
        ++count;
    }

    void unlock()
    {
        if (!count)
            qFatal("QtopiaSharedMemoryLock: unlock without lock");
        if (--count)
            return;
        struct sembuf op;
        op.sem_num = 0;
        op.sem_op = type == Write ? MaxReaders : 1;
        op.sem_flg = SEM_UNDO;
        int result;
        do {
            result = semop(semId, &op, 1);
        } while (result == -1 && errno == EINTR);
        if (result == -1)
            qFatal("QtopiaSharedMemoryLock: unlock failed: %s", strerror(errno));
    }

private:
    Q_DISABLE_COPY(QtopiaSharedMemoryLock)
    int semId;
    int count;
    Type type;
    bool owner;
};

class QtopiaSharedMemoryLockHandle
{
public:
    QtopiaSharedMemoryLockHandle(QtopiaSharedMemoryLock *lock,
                                QtopiaSharedMemoryLock::Type type)
        : lockPtr(lock) { lockPtr->lock(type); }
    ~QtopiaSharedMemoryLockHandle() { lockPtr->unlock(); }
private:
    Q_DISABLE_COPY(QtopiaSharedMemoryLockHandle)
    QtopiaSharedMemoryLock *lockPtr;
};

#endif
