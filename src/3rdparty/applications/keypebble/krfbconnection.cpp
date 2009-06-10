#include <assert.h>
#include <QTcpSocket>
#include <qtimer.h>
#include <string.h>

#include "krfbconnection.h"
#include "krfblogin.h"
#include "krfboptions.h"
#include "krfbdecoder.h"
#include "krfbbuffer.h"

KRFBConnection::KRFBConnection( QObject *parent )
    : QObject( parent )
{
  portBase_ = 5900;
  currentState_ = Disconnected;
  sock = 0;
  minData_ = 0;
  options_ = new KRFBOptions();
  updater = 0;
  decoder_ = 0;
  buffer_ = 0;

  login = 0;
}

KRFBConnection::~KRFBConnection()
{
  if ( ( currentState_ != Disconnected ) && ( currentState_ != Disconnecting ) && sock ) {
    disconnectDone();
  }
  delete options_;
}

void KRFBConnection::connectTo( const QString &host, int display )
{
  qWarning("void KRFBConnection::connectTo() %s, %d",host.toLocal8Bit().constData(),display);

  if ( currentState_ != Disconnected );
    disconnect();

  this->host_= host;
  this->display_ = display;

  sock = new QTcpSocket( this );

  // Connect to something to notice connection or error
  connect( sock, SIGNAL( error(QAbstractSocket::SocketError) ), SLOT( gotSocketError(QAbstractSocket::SocketError) ) );
  connect( sock, SIGNAL( connected() ), SLOT( gotSocketConnection() ) );

  qWarning( "Connecting..." );

  currentState_ = Connecting;
  sock->connectToHost( host_, portBase_ + display_ );
}

void KRFBConnection::disconnect()
{
  qWarning( "void KRFBConnection::disconnect()" );

  if ( ( currentState_ != Disconnected )
       && ( currentState_ != Disconnecting )
       && sock ) {
    currentState_ = Disconnecting;

    //connect( sock, SIGNAL( disconnected() ), SLOT( disconnectDone() ) );
    sock->close();

    if ( sock->state() != QAbstractSocket::ClosingState )
      disconnectDone();
  }
}

void KRFBConnection::disconnectDone()
{
  qWarning( "KRFBConnection disconnected" );
  currentState_ = Disconnected;
  delete sock;
  sock = 0;
  minData_ = 0;
  delete updater;
  delete decoder_;
  delete buffer_;
  delete login;

  updater = 0;
  decoder_ = 0;
  buffer_ = 0;
  login = 0;
  emit disconnected();
}

void KRFBConnection::gotSocketConnection()
{
  qWarning( "Connected, logging in..." );
  currentState_ = LoggingIn;

  static QString statusMsg = tr( "Connected" );
  emit statusChanged( statusMsg );

  // Do some login stuff
  login = new KRFBLogin( this );
}

void KRFBConnection::gotRFBConnection()
{
  qWarning( "Logged into server" );

  currentState_ = Connected;
  emit connected();

  // Create the decoder and start doing stuff
  decoder_ = new KRFBDecoder( this );

  buffer_ = new KRFBBuffer( decoder_ ); // No tr
  decoder_->setBuffer( buffer_ );

  connect( decoder_, SIGNAL( status(const QString&) ),
           this, SIGNAL( statusChanged(const QString&) ) );
  emit loggedIn();

  decoder_->start();

  updater = new QTimer;
  connect( updater, SIGNAL( timeout() ), SLOT( updateTimer() ) );
  updater->start( options_->updateRate );
}

void KRFBConnection::gotSocketError( QAbstractSocket::SocketError rrr )
{
  int errno = (int) rrr;
  currentState_ = Error;

  // Do some error handling stuff
  qWarning( "KRFBConnection: Socket error %d", errno );

  static QString refused = tr( "Connection Refused" );
  static QString host = tr( "Host not found" );
  static QString read = tr( "Read Error: QAbstractSocket reported an error reading\n"
			      "data, the remote host has probably dropped the\n"
			      "connection." );
  static QString confused = tr( "QAbstractSocket reported an invalid error code" );

  QString msg;
  switch ( errno ) {
  case QAbstractSocket::ConnectionRefusedError:
    msg = refused;
    break;
  case QAbstractSocket::HostNotFoundError:
    msg = host;
    break;
  case QAbstractSocket::RemoteHostClosedError:
    return;
    break;
  default:
    msg = confused;
  };

  QObject::disconnect( sock, SIGNAL( readyRead() ), this, SLOT( gotMoreData() ) );
  delete sock;
  sock = 0;
    //setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  currentState_ = Disconnected;

  emit error( msg );
}

int KRFBConnection::bytesAvailable() const
{
    return sock->size();
}

void KRFBConnection::gotMoreData()
{
  assert( minData_ > 0 );

  if ( sock->size() >= minData_ ) {
    minData_ = 0;
    QObject::disconnect( sock, SIGNAL( readyRead() ), this, SLOT( gotMoreData() ) );
    emit gotEnoughData();
  }
}

void KRFBConnection::waitForData( unsigned int sz )
{
  assert( minData_ == 0 );
  assert( sz > 0 );
  assert( currentState_ != Error );
   
  if ( sock->size() >= sz ) {
      //    qWarning( "No need to wait for data" );
    emit gotEnoughData();
  }
  else {
      //    qWarning( "Waiting for %u bytes", sz );
    
    minData_ = sz;
    connect( sock, SIGNAL( readyRead() ), SLOT( gotMoreData() ) );
  }
}

int KRFBConnection::read( void *buf, int sz )
{
  return sock->read( (char *)  buf, sz );
}

int KRFBConnection::write( void *buf, int sz )
{
  return sock->write( (const char *) buf, sz );
}

KRFBConnection::State KRFBConnection::state() const
{
  return currentState_;
}

void KRFBConnection::setPortBase( int base )
{
  portBase_ = base;
}

int KRFBConnection::portBase() const
{
  return portBase_;
}

void KRFBConnection::setPassword( const QString &pass )
{
  this->pass_ = pass;
}

void KRFBConnection::updateTimer()
{
  decoder_->sendUpdateRequest( true );
}

void KRFBConnection::refresh()
{
  decoder_->sendUpdateRequest( false );
}

void KRFBConnection::sendCutText( const QString &text )
{
  decoder_->sendCutEvent( text );
}

const QUrl &KRFBConnection::url()
{
  url_.setPort( display() );
  url_.setHost( host() );
  url_.setPath( "/" );

  return url_;
}
