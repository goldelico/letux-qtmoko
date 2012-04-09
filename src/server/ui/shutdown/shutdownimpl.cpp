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

#include "shutdownimpl.h"
#include "applicationlauncher.h"

#include <qtimer.h>
#include <qprogressbar.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <QtopiaChannel>
#include <QDir>
#include <QFile>
#include <QMessageBox>

#include <stdio.h>

using namespace Ui;

// Enables gta04 initramfs menu
static void enableBootmenu()
{
    QFile::remove("/boot/gta04-init/bootdev");
}

// Disables gta04 initramfs menu
static void disableBootmenu()
{
    QFile::copy("/boot/gta04-init/lastbootdev", "/boot/gta04-init/bootdev");
}

static QString bootTo(int index, QStringList & distroList)
{
    if(index == 0) {
        disableBootmenu();
        return "";
    }
    
    if(index == 1) {
        enableBootmenu();
        return "";
    }
    index -= 2;
    
    if(index < 0 || index > distroList.count()) {
        return "invalid index";
    }

    QFile lastDev("/boot/gta04-init/lastbootdev");
    if(!lastDev.open(QIODevice::ReadOnly)) {
        return lastDev.fileName() + " open failed: " + lastDev.errorString();
    }
    QByteArray content = lastDev.readAll();
    lastDev.close();
    
    for(int i = 0; i < content.count(); i++) {
        if(content.at(i) <= 32) {
            content = content.left(i);
            break;
        }
    }
    
    content = content + " /distros/" + distroList.at(index).toLatin1();

    QFile bootDev("/boot/gta04-init/bootdev");
    if(!bootDev.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        return bootDev.fileName() + " open failed: " + bootDev.errorString();
    }

    if(bootDev.write(content) != content.count()) {
        bootDev.close();
        return bootDev.errorString();
    }

    bootDev.close();
    return "";
}

ShutdownImpl::ShutdownImpl( QWidget* parent, Qt::WFlags fl )
    : QDialog( parent, fl )
{
    setupUi(this);
    timer = new QTimer( this );
    connect( timer, SIGNAL(timeout()), this, SLOT(timeout()) );

    connect( reboot, SIGNAL(clicked()), this, SLOT(rebootClicked()));
    connect( restart, SIGNAL(clicked()), this, SLOT(restartClicked()));
    connect( quit, SIGNAL(clicked()), this, SLOT(quitClicked()));
    connect( shut, SIGNAL(clicked()), this, SLOT(shutdownClicked()));

//    connect( cancel, SIGNAL(clicked()), this, SLOT(cancelClicked()) );

    progressBar->hide();
    QtopiaApplication::hideInputMethod();
#ifdef QT_QWS_SL5XXX
    QPushButton *sb = Shutdown::shutdown;
    sb->hide();
#endif

    // Bootmenu on GTA04, described here: https://github.com/radekp/gta04-init
    bool hasBootmenu = QDir("/boot/gta04-init").exists();
    bootmenuCombo->setVisible(hasBootmenu);
    if(hasBootmenu) {
        disableBootmenu();  // normally we want the bootmenu disabled
        
        QDir d("/distros");
        distroList = d.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
        bootmenuCombo->addItems(distroList);
    }
    connect(bootmenuCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(bootmenuIndexChanged(int)));
}

void ShutdownImpl::rebootClicked()
{
    operation = QtopiaServerApplication::RebootSystem;
    initiateAction();
}
void ShutdownImpl::restartClicked()
{
    operation = QtopiaServerApplication::RestartDesktop;
    initiateAction();
}
void ShutdownImpl::quitClicked()
{
    operation = QtopiaServerApplication::TerminateDesktop;
    initiateAction();
}
void ShutdownImpl::shutdownClicked()
{
    operation = QtopiaServerApplication::ShutdownSystem;
    initiateAction();
}

void ShutdownImpl::initiateAction()
{
    progress = 0;

    progressBar->show();
    timer->start( 300 );
    timeout();
}

/*
void ShutdownImpl::cancelClicked()
{
    progressBar->hide();
    if ( timer->isActive() )
        timer->stop();
    else
        close();
}
*/

void ShutdownImpl::timeout()
{
    if ( (progress+=2) > progressBar->maximum() ) {
        progressBar->hide();
        timer->stop();
        emit shutdown( operation );
        close();
    } else {
        progressBar->setValue( progress );
    }
}

void ShutdownImpl::bootmenuIndexChanged(int index)
{
    QString err = bootTo(index, distroList);
    if(err.length() > 0) {
        qWarning() << "Boot selection failed: " << err;
    }
}

/*!
  \class ShutdownDialogTask
    \inpublicgroup QtBaseModule
  \ingroup QtopiaServer::GeneralUI
  \brief The ShutdownDialogTask class provides a shutdown dialog that allows the user to control 
  system restarts and shutdowns.

  The ShutdownDialogTask provides a Qt Extended Server Task.  Qt Extended Server Tasks 
  are documented in full in the QtopiaServerApplication class documentation.

  The shutdown dialog is controlled by this class and allows users to terminate Qtopia, restart Qtopia, shutdown
  the system or reboot the system.  The default implementation is a simple
  dialog allowing the user to select from one of these four options.

  The shutdown dialog appears in response to the
  QtopiaServerApplication::shutdownRequested() signal.  Once the user has
  selected their desired option, the dialog invokes the
  QtopiaServerApplication::shutdown() method and closes.

  This class is part of the Qt Extended server and connot be used by other Qt Extended applications.
*/

/*!
  \internal
*/
ShutdownDialogTask::ShutdownDialogTask( QObject* parent ) 
    : QObject(parent)
{
    QObject::connect(QtopiaServerApplication::instance(), SIGNAL(shutdownRequested()),
                     this, SLOT(shutdownRequested()));
}

/*!
  This slot is invoked in response to the QtopiaServerApplication::shutdownRequested() signal.
*/
void ShutdownDialogTask::shutdownRequested()
{
    ShutdownImpl *sd = new ShutdownImpl(0);
    sd->setAttribute(Qt::WA_DeleteOnClose);
    QObject::connect(sd, SIGNAL(shutdown(QtopiaServerApplication::ShutdownType)),
            QtopiaServerApplication::instance(), SLOT(shutdown(QtopiaServerApplication::ShutdownType)));
    sd->showMaximized();
}

QTOPIA_TASK(ShutdownDialogTask,ShutdownDialogTask);

// "shutdown" builtin
static QWidget *shutdown()
{
    QtopiaChannel::send( "QPE/System", "shutdown()" );
    return 0;
}
QTOPIA_SIMPLE_BUILTIN(shutdown, shutdown);
