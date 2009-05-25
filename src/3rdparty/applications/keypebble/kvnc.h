// -*- c++ -*-

#ifndef KVNC_H
#define KVNC_H

#include <qmainwindow.h>
#include <qurl.h>

class QAction;
class KRFBCanvas;
class QPushButton;
class QToolBar;

/**
 * Top level window for Keystone.
 *
 * @author Richard Moore, rich@kde.org
 * @version $Id: kvnc.h,v 1.1 2001/09/07 02:15:52 martinj Exp $
 */
class KVNC : public QMainWindow
{
  Q_OBJECT
public:
  KVNC( QWidget* parent = 0, Qt::WFlags fl=0);  
  ~KVNC();

public slots:
  void openURL( const QUrl & );
  void connectToServer();
  void showOptions();

protected:
  void setupActions();

protected slots:
  void connected();
  void loggedIn();
  void disconnected();
  void statusMessage( const QString & );
  void error( const QString & );
  void zoom( bool );

private:
  bool isConnected;
  KRFBCanvas *canvas;
  QMenu *cornerMenu;
  QPushButton *cornerButton;
  QAction *optionsAction;
  QAction *connectAction;
  QAction *zoomAction;
};

#endif // KVNC_H
