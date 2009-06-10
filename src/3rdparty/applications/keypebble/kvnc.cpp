#include <qdialog.h>
#include <qpixmap.h>
#include <qdom.h>
#include <qaction.h>
#include <qstatusbar.h>
#include <qpushbutton.h>
#include <qtimer.h>
#include <qmessagebox.h>
#include <QtGui/qscreen_qws.h>
#include <qtopiaapplication.h>

#include <QMenu>
#include <qsoftmenubar.h>

#include <assert.h>

#include "kvnc.h"
#include "krfbcanvas.h"
#include "kvncoptionsdlg.h"
#include "krfbconnection.h"
#include "krfboptions.h"

const int StatusTextId = 0;

/*!
  \class KVNC
  \brief The KVNC class is a VNC client.

  \legalese
    Keypebble, the VNC client for Qtopia, is based on Keystone from
    the KDE project. Keystone and Keypebble are distributed
    under the terms of the GNU General Public License. The VNC server
    code in Qt/Embedded is copyright Trolltech.

    A primary copyright holder is \l {mailto:rich@kde.org}{Richard Moore}.

    This software owes a lot to Markus Wuebben's kvncviewer,
    which some of the code is derived from. Thanks also go to ORL for
    making the RFB protocol and servers GPL.

  \ingroup qtopia3rdparty
*/

// QDOC_SKIP_BEGIN

/*! \internal */

KVNC::KVNC( QWidget* parent, Qt::WFlags fl) :
QMainWindow( parent, fl )
{
    setWindowTitle( tr("keypebble") );
    isConnected = false;

    canvas = new KRFBCanvas( this ); // No tr
    setCentralWidget( canvas );

    connect( canvas->connection(), SIGNAL(statusChanged(const QString&)),
	    this, SLOT(statusMessage(const QString&)) );
    connect( canvas->connection(), SIGNAL(error(const QString&)),
	    this, SLOT(error(const QString&)) );
    connect( canvas->connection(), SIGNAL(connected()), this, SLOT(connected()) );
    connect( canvas->connection(), SIGNAL(loggedIn()), this, SLOT(loggedIn()) );
    connect( canvas->connection(), SIGNAL(disconnected()), this, SLOT(disconnected()) );

    setupActions();

    if ( qApp->argc() > 1 ) {
        openURL( QUrl( qApp->argv()[1]) );
    } else {
        QTimer::singleShot( 0, canvas, SLOT(openConnection()) );
    }
}

/*! */

KVNC::~KVNC()
{

}

/*! \a url */

void KVNC::openURL( const QUrl &url )
{
    canvas->openURL( url );
}

/*! */

void KVNC::setupActions()
{
    cornerMenu = QSoftMenuBar::menuFor(this);

    connectAction = new QAction(tr("Connect"), this);
    connect(connectAction, SIGNAL(triggered()), this, SLOT(connectToServer()));
    cornerMenu->addAction(connectAction);

    zoomAction = new QAction(tr("Zoom"), this);
    zoomAction->setCheckable(true);
    connect(zoomAction, SIGNAL(toggled(bool)), this, SLOT(zoom(bool)));
    cornerMenu->addAction(zoomAction);

    optionsAction = new QAction(tr("Settings..."), this);
    connect(optionsAction, SIGNAL(triggered()), this, SLOT(showOptions()));
    cornerMenu->addAction(optionsAction);

    QSoftMenuBar::setLabel(this, Qt::Key_Back, QSoftMenuBar::Back);
    QSoftMenuBar::setHelpEnabled( this, true );
}

/*! */
void KVNC::connectToServer()
{
    if ( isConnected ) {
      canvas->closeConnection();
      isConnected = false;
    } else {
      canvas->openConnection();
    }
}

/*! \a z */

void KVNC::zoom( bool z )
{
    if (isConnected) {
	canvas->setViewScale( z ? 2 : 1 );
    }
}

/*! */

void KVNC::connected()
{
    static QString msg = tr( "Connected to remote host" );
    statusMessage( msg );
    zoomAction->setEnabled( true );
    connectAction->setText( tr("Disconnect") );
    isConnected = true;
}

/*! */

void KVNC::loggedIn()
{
    static QString msg = tr( "Logged in to remote host" );
    statusMessage( msg );
}

/*! */

void KVNC::disconnected()
{
    static QString msg = tr( "Connection closed" );
    statusMessage( msg );

    isConnected = false;
    zoomAction->setEnabled( false );
    connectAction->setText( tr("Connect...") );
}

/*! \a m */

void KVNC::statusMessage( const QString &m )
{
    Qtopia::statusMessage( m );
}

/*! \a msg */

void KVNC::error( const QString &msg )
{
    QMessageBox *box = new QMessageBox(QObject::tr("VNC Viewer"), msg,
                                       QMessageBox::Information, 0, 0, QMessageBox::Ok);
    QtopiaApplication::execDialog(box);

    delete box;
}

/*! */

void KVNC::showOptions()
{
    KVNCOptionsDlg *wdg = new KVNCOptionsDlg( canvas->connection()->options() );
    if ( QtopiaApplication::execDialog(wdg) == QDialog::Accepted ) {
	canvas->connection()->options()->save();
    }
    delete wdg;
}

// QDOC_SKIP_END
