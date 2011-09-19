/* -------------------------------------------------------------------------- */
/*                                                                            */
/* [MyPty.h]                 Pseudo Terminal Device                           */
/*                                                                            */
/* -------------------------------------------------------------------------- */
/*                                                                            */
/* Copyright (c) 1997,1998 by Lars Doelle <lars.doelle@on-line.de>            */
/*                                                                            */
/* This file is part of Konsole - an X terminal for KDE                       */
/*                                                                            */
/* -------------------------------------------------------------------------- */
/*									      */
/* Ported Konsole to Qt/Embedded                                              */
/*									      */
/* Copyright (C) 2000 by John Ryland <jryland@trolltech.com>                  */
/*									      */
/* -------------------------------------------------------------------------- */

/*! \file
*/

#ifndef MY_PTY_H
#define MY_PTY_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QSize>
#include <QThread>

#include "io_layer.h"

typedef QMap<QString, QString> EnvironmentMap;

class Profile;
class QSocketNotifier;
class MyPty : public IOLayer
{
    Q_OBJECT
public:

    MyPty(const Profile&);
    ~MyPty();



    QString identifier()const;
    QString name()const;
    QBitArray supports()const;
    bool isRunning();
    void setWorkingDirectory(QString);
    void setUtf8Mode(bool);
    void setErase(char);
    void setWriteable(bool );

  public slots:
 /*!
      having a `run' separate from the constructor allows to make
      the necessary connections to the signals and slots of the
      instance before starting the execution of the client.
    */
    int start();
    int run(const char* pgm, QStringList & args , const char* term, int addutmp);
    bool open();
    void close();
    void reload( const Profile& );
    void setWindowSize(int lines, int columns);
    QSize windowSize();
    int pid();
    void error();
    bool isConnected() { return true; };
  signals:

    /*!
        emitted when the client program terminates.
        \param status the wait(2) status code of the terminated client program.
    */
    void done(int status);

    /*!
        emitted when a new block of data comes in.
        \param s - the data
        \param len - the length of the block
    */
    void received(const char *, int);

  public slots:

    void send(const char *, int );

private:
    const char* deviceName();
    bool _running;

protected slots:
      void readPty();
      void donePty();

private:
    int  openPty();

private:

    char m_ptynam[16]; // "/dev/ptyxx" | "/dev/ptmx"
    char m_ttynam[16]; // "/dev/ttyxx" | "/dev/pts/########..."
    int m_fd;
    int m_cpid;
    QSocketNotifier* m_sn_e;
    QSocketNotifier* m_sn_r;
    char* m_term;
    QSize _size;

    QString m_cmd;
    EnvironmentMap m_env;
};

class Sleeper : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
        QThread::msleep(msecs);
    }
};

#endif
