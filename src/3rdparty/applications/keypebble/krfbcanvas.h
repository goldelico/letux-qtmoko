// -*- c++ -*-

#ifndef KRFBCANVAS_H
#define KRFBCANVAS_H

#include <QScrollArea>
#include <QPixmap>
#include <QLabel>

#include <qurl.h>

class KRFBConnection;
class QTimer;

/**
 * Displays data from an KRFBDecoder, and sends events to the
 * KRFBConnection.
 */
class KRFBCanvas : public QScrollArea
{
  Q_OBJECT
public:
  KRFBCanvas( QWidget *parent = 0);
  ~KRFBCanvas();

  void setConnection( KRFBConnection * );
  KRFBConnection *connection() { return connection_; };
  void setViewScale( int s );

public slots:
  void openConnection();
  void openURL( const QUrl & );
  void closeConnection();
  void passwordRequired( KRFBConnection * );

  void refresh();
  void bell();

protected:
  virtual void keyPressEvent( QKeyEvent * );
  virtual void keyReleaseEvent( QKeyEvent * );
  virtual void mousePressEvent( QMouseEvent * );
  virtual void mouseReleaseEvent( QMouseEvent * );
  virtual void mouseMoveEvent( QMouseEvent * );
  virtual void paintEvent( QPaintEvent *e );
  virtual void scrollContentsBy( int x, int y );

protected slots:
  void loggedIn();
  void viewportUpdate( int x, int y, int w, int h );
  void clipboardChanged();
  void doAutoScroll();
  void updateSizeChanged(int, int);

private:
  KRFBConnection *connection_;
  QString password;
  bool loggedIn_;
  int dx;
  int dy;
  QMouseEvent *autoMouseEvent;
  QTimer *autoTimer;
};

#endif // KRFBCANVAS_H
