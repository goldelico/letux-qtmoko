/* See the readme for legal */

#include <QtCore/QDir>
#include <QtCore/QDebug>

#ifdef QTOPIA_PHONE
#include <qtopia/QSoftMenuBar>
#include <QtopiaApplication>
#else
#include <QtGui/QMenuBar>
#endif

#include <QtGui/QLayout>
#include <QtGui/QMenu>
#include <QtGui/QBoxLayout>

#include "TerminalDisplay.h"
//#include "KeyboardTranslator.h"
#include "profile.h"
#include "ColorScheme.h"
#include "Preferences.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags f)
   : QMainWindow(parent,f)
{
  _sessionCount = 0;
  // setup the ui
#ifdef QTOPIA_PHONE
  QMenu *m = QSoftMenuBar::menuFor(this);
#else
  resize(800,600);
  QMenu *m = menuBar()->addMenu(tr("Options"));
#endif

  acNewSession = m->addAction(tr("New Session"));
  acCloseSession = m->addAction(tr("Close Session"));
  acPreferences = m->addAction(tr("Preferences"));
#ifndef QTOPIA_PHONE
  m->addSeparator();
  QAction *m_exit = m->addAction(tr("Exit"));
  connect(m_exit, SIGNAL(triggered()), this, SLOT(closeAll()));
#endif
// FIXME: need this to close, and hide the input method
#ifdef QTOPIA_APPLICATION
  connect(QtopiaApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(closeAll()));
#else
  connect(QApplication::instance(), SIGNAL(aboutToQuit()), this, SLOT(closeAll()));
#endif
  connect(acNewSession, SIGNAL(triggered()), this, SLOT(newSession()));
  connect(acCloseSession, SIGNAL(triggered()), this, SLOT(closeSession()));
  connect(acPreferences, SIGNAL(triggered()), this, SLOT(showPreferences()));
  tabs = new QTabWidget();
   
  setCentralWidget(tabs);
  setWindowTitle(tr("qterminal"));
#ifdef QTOPIA_PHONE
  if (Qtopia::mousePreferred) {
    // not sure which one is preferrable
    //QtopiaApplication::setInputMethodHint(this, QtopiaApplication::AlwaysOn);
    setAttribute(Qt::WA_InputMethodEnabled, true);
    QtopiaApplication::instance()->showInputMethod();
  }
#endif
  connect(tabs,SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));
  newSession();
}

MainWindow::~MainWindow()
{

}

void MainWindow::addSession(const char *name)
{

  TerminalDisplay *display = new TerminalDisplay();
  Profile *profile = new Profile(name);
  Session *session = new Session(*profile);
  const ColorScheme* colorScheme = ColorSchemeManager::instance()->findColorScheme(profile->colorScheme());
  //TODO Temporary settings used here
  display->setBellMode(0);
  display->setTerminalSizeHint(false);
  display->setTripleClickMode(TerminalDisplay::SelectWholeLine);
  display->setTerminalSizeStartup(false);
  display->setScrollBarPosition(TerminalDisplay::ScrollBarRight);
  display->setRandomSeed(session->sessionId() * 31);
  display->setVTFont(QFont(profile->fontName(), profile->fontSize()));
  //ColorSchemeManager::instance()->addColorScheme(new ColorScheme(*colorScheme));
  ColorEntry table[TABLE_COLORS];
  colorScheme->getColorTable(table, display->randomSeed());
  display->setColorTable(table);
  display->setOpacity(colorScheme->opacity());
  display->setKeyboardCursorShape((TerminalDisplay::KeyboardCursorShape)profile->cursorShape());
  
  session->addView(display);

  int tabid = tabs->addTab(display,QIcon(":icon/konsole"), tr(name));
  session->setTitle(Session::NameRole,name);
  session->setProgram(profile->command());
  session->setHistoryType( HistoryTypeBuffer(profile->historyLines()) );
  session->setProfileKey(profile->name());

  session->setInitialWorkingDirectory(QDir::homePath());

  connect(session, SIGNAL(finished(QObject*)), this, SLOT(finished(QObject*)));
  session->setKeyBindings(profile->keyboard());
  _sessionCount++;
  _Sessions.append(session);
  session->run();
  tabs->setCurrentIndex(tabid);
  display->setFocus(Qt::OtherFocusReason);
}

void MainWindow::finished(QObject *obj)
{
  Session *session = (Session *)obj;
  _sessionCount--;
  int tabid = tabs->indexOf(session->views()[0]);
  tabs->removeTab(tabid);   
  _Sessions.removeAt(tabid);
  //is it done automagically
  //delete session;
    
  if (_sessionCount<=0) {
// FIXME: this seems to me more like a hack than a real solution ...
#ifdef QTOPIA_PHONE
   if (Qtopia::mousePreferred) {
     QtopiaApplication::instance()->hideInputMethod();
   }
#endif
    close();
  }
}

void MainWindow::currentChanged(int)
{
  if(tabs->currentIndex() >= 0) {
    tabs->currentWidget()->setFocus(Qt::OtherFocusReason);
  }
}

void MainWindow::newSession()
{
  //QString title = tr("Shell n.")+QString::number(_sessionCount+1);
  QString title = tr("Shell");
  addSession(title.toLatin1().constData());
}

void MainWindow::closeSession()
{
  if (tabs->currentIndex()>=0) {
    Session *session = _Sessions.at(tabs->currentIndex());
    session->close();
  }
}

void MainWindow::closeAll()
{
  // close all sessions
  while (_Sessions.count()>0) {
    Session *session = _Sessions.at(0);
    session->close();
  }
  // this should never happen right now...
  close();
}

void MainWindow::showPreferences()
{
  Session *sess =  _Sessions.at(tabs->currentIndex());
  Profile *prof = new Profile(sess->profileKey());
  Preferences *pd = new Preferences(prof);
  pd->setModal(true);
#ifdef QTOPIA_PHONE
  if (Qtopia::mousePreferred)
     QtopiaApplication::instance()->hideInputMethod();
#endif
  pd->exec();
  if (pd->result()==QDialog::Accepted) {
    // apply the changes and update the current session settings
    Profile *p = pd->profile();
    TerminalDisplay *display = sess->views()[0];
    display->setVTFont(QFont(p->fontName(), p->fontSize()));
const ColorScheme* colorScheme = ColorSchemeManager::instance()->findColorScheme(p->colorScheme());
    ColorEntry table[TABLE_COLORS];
    colorScheme->getColorTable(table, display->randomSeed());
    display->setColorTable(table);
    display->setOpacity(colorScheme->opacity());
    display->setKeyboardCursorShape((TerminalDisplay::KeyboardCursorShape)p->cursorShape());
    sess->setKeyBindings(p->keyboard());
    sess->setHistoryType(HistoryTypeBuffer(p->historyLines()));
    sess->setProfileKey(p->name());
    p->saveProfile(p->name());    
  }
#ifdef QTOPIA_PHONE
  if (Qtopia::mousePreferred) 
     QtopiaApplication::instance()->showInputMethod();
#endif
  delete pd;
}
