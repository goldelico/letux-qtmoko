#include "kvncconnectdlg.h"
#include "krfbconnection.h"
#include "krfbcanvas.h"
#include "krfboptions.h"
#include "krfbbuffer.h"

#include <qsettings.h>

#include <qapplication.h>
#include <qclipboard.h>
#include <qaction.h>
#include <qpixmap.h>
#include <qapplication.h>
#include <qmainwindow.h>
#include <qtimer.h>

#include <QPainter>
#include <QPaintEvent>
#include <QRect>
#include <QBitmap>
#include <QDebug>
#include <QScrollBar>

KRFBCanvas::KRFBCanvas( QWidget *parent )
  : QScrollArea( parent )
{
    connection_ = new KRFBConnection();
    connect( connection_, SIGNAL( passwordRequired(KRFBConnection*) ),
	    this, SLOT( passwordRequired(KRFBConnection*) ) );
    connect( connection_, SIGNAL( loggedIn() ),
	    this, SLOT( loggedIn() ) );

    loggedIn_ = false;

    setWidget(new QFrame);
    setProperty("updateOnEditFocus", true);

    setBackgroundRole( QPalette::Dark );
    viewport()->setBackgroundRole( QPalette::Dark );
    widget()->setMinimumSize(320,240);

    autoTimer = new QTimer( this );
    connect( autoTimer, SIGNAL(timeout()), this, SLOT(doAutoScroll()) );
    dx = dy = 0;
}

KRFBCanvas::~KRFBCanvas()
{
}

void KRFBCanvas::setViewScale( int s )
{
    connection_->buffer()->setScaling( s );
    repaint();
}

void KRFBCanvas::openConnection()
{
    KVNCConnectDlg dlg( connection_, this ); // No tr
    if ( dlg.exec() ) {
        QString host = dlg.hostname().toLatin1();
        password = dlg.password();
        connection_->connectTo( host, dlg.display() );
    }
}

void KRFBCanvas::openURL( const QUrl &url )
{
    if ( loggedIn_ ) {
        qWarning( "openURL invoked when logged in\n" );
        return;
    }

    QString host = url.host().toLatin1();
    int display = url.port();
    connection_->setPassword( url.password().toLatin1() );
    connection_->connectTo( host, display );
}

void KRFBCanvas::closeConnection()
{
    loggedIn_ = false;
    connection_->disconnect();
}

void KRFBCanvas::passwordRequired( KRFBConnection *con )
{
    con->setPassword( password.toLatin1() );
}

void KRFBCanvas::bell()
{
    if ( connection_->options()->deIconify ) {
        topLevelWidget()->raise();
        topLevelWidget()->show();
    }
}

void KRFBCanvas::updateSizeChanged(int w, int h)
{
    widget()->setMinimumSize(w,h);
    widget()->resize(w, h);
    viewportUpdate(0, 0, w, h);
}

void KRFBCanvas::loggedIn()
{
    qWarning( "Ok, we're logged in" );

    //
    // Get ready for action
    //
    loggedIn_ = true;
    viewport()->setMouseTracking( true );

    // Start using the buffer
    connect( connection_->buffer(), SIGNAL( sizeChanged(int,int) ),
           this, SLOT( updateSizeChanged(int,int) ) );
    connect( connection_->buffer(), SIGNAL( updated(int,int,int,int) ),
           this, SLOT( viewportUpdate(int,int,int,int) ) );
    connect( connection_->buffer(), SIGNAL( bell() ),
           this, SLOT( bell() ) );
    connect( qApp->clipboard(), SIGNAL( dataChanged() ),
           this, SLOT( clipboardChanged() ) );
}

void KRFBCanvas::paintEvent( QPaintEvent *e )
{
    QPainter p(viewport());
    QRect r = e->rect();

    if ( loggedIn_ ) {
        QRegion bgr = e->region();
	for ( uint i = 0; i < (uint)bgr.rects().count(); i++ ) {
	    p.fillRect( bgr.rects()[i], palette().dark() );
	}
	connection_->buffer()->paint( &p, dx,
			dy, r.width(), r.height() );
    } else {
        p.fillRect(0, 0, 800, 600, palette().dark());
    }
    viewport()->update();
}

void KRFBCanvas::viewportUpdate( int x, int y, int w, int h )
{
    widget()->repaint(x,y,w,h);
}

void KRFBCanvas::mousePressEvent( QMouseEvent *e )
{
    if ( loggedIn_ ) {
        QMouseEvent *f;
        f = new QMouseEvent(e->type(), QPoint(e->x()+dx,e->y()+dy), e->button(), 
			e->buttons(), e->modifiers());
	connection_->buffer()->mouseEvent( f );
    }
}

void KRFBCanvas::mouseReleaseEvent( QMouseEvent *e )
{
    if ( loggedIn_ ) {
        QMouseEvent *f;
        f = new QMouseEvent(e->type(), QPoint(e->x()+dx,e->y()+dy), e->button(), 
			e->buttons(), e->modifiers());
	connection_->buffer()->mouseEvent( f );
	//autoTimer->stop();
    }
}

void KRFBCanvas::mouseMoveEvent( QMouseEvent *e )
{
    if ( loggedIn_ ) {
        QMouseEvent *f;
        f = new QMouseEvent(e->type(), QPoint(e->x()+dx,e->y()+dy), e->button(), 
			e->buttons(), e->modifiers());
	connection_->buffer()->mouseEvent( f );
    }
    if ( loggedIn_ & (Qt::LeftButton|Qt::RightButton|Qt::MidButton) ) {
	//autoTimer->stop();
	QPoint vpos = mapToGlobal( e->pos() );
	if ( vpos.x() >= viewport()->width()-1 && e->pos().x() < width()-1 ) {
            dx = 8;
	} else if ( vpos.x() <= 0 && e->pos().x() > 0 ) {
	    dx = -8;
	}
	if ( vpos.y() >= viewport()->height()-1 && e->pos().y() < height()-1 ) {
	    dy = 8;
	} else if ( vpos.y() <= 0 && e->pos().y() > 0 ) {
	    dy = -8;
	}
	//if ( dx || dy ) {
	//    autoMouseEvent = new QMouseEvent(e->type(), vpos, e->button(), (Qt::KeyboardModifiers)0);
	//    autoTimer->start( 50, TRUE );
	//}
    }
}

void KRFBCanvas::keyPressEvent( QKeyEvent *e )
{
    switch ( e->key() ) {
        case Qt::Key_Back:
          QCoreApplication::exit(0);
          break;
        default:
          if ( loggedIn_ )
              connection_->buffer()->keyPressEvent( e );
          break;
    }
}

void KRFBCanvas::keyReleaseEvent( QKeyEvent *e )
{
    if ( loggedIn_ )
        connection_->buffer()->keyReleaseEvent( e );
}

void KRFBCanvas::scrollContentsBy( int x, int y )
{
    dx=dx - x;
    dy=dy - y;
    viewport()->update();
}

void KRFBCanvas::refresh()
{
    if ( loggedIn_ )
        connection_->refresh();
}

void KRFBCanvas::clipboardChanged()
{
    if ( loggedIn_ ) 
        connection_->sendCutText( qApp->clipboard()->text() );
}

void KRFBCanvas::doAutoScroll()
{
    if ( dx || dy ) {
	//scrollBy( dx, dy );
	//QApplication::postEvent( viewport(), autoMouseEvent );
	//dx = 0;
	//dy = 0;
    }
}

