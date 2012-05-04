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
  QSoftMenuBar::setLabel(this, Qt::Key_Back, QSoftMenuBar::NoLabel);
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
 
  centralWidget = new QWidget(this);
  tabs = new QTabWidget();
  firstDisplay = NULL;
 
  setCentralWidget(centralWidget);
  
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
  
   if(QApplication::argc() == 1)
       addSession("Shell", QString(), QStringList());
   else {
       QString cmd = "/bin/bash";
       QStringList args;
       
       // Parse arguments and make cmd and args for launched process
       // These 2 cases should be handled:
       //
       // qterminal -c ls /
       // qterminal /path/to/script.sh
       //
       for(int i = 1; i < QApplication::argc(); i++) {
           QString arg = QApplication::argv()[i];
           if(i == 1 && arg != "-c") {
                cmd = arg;                  // e.g. qterminal /path/to/script.sh
                continue;
           }
           args.append(QApplication::argv()[i]);
       }       
       addSession("Shell", cmd, args);
   }
}

MainWindow::~MainWindow()
{

}

void MainWindow::setDocument(const QString & filename)
{
    addSession(filename.toLatin1().constData(), filename, QStringList());
}

void MainWindow::addSession(const char *name, const QString & cmd, QStringList args)
{

  TerminalDisplay *display = new TerminalDisplay();
  Profile *profile = new Profile(name);
  Session *session = new Session(*profile, cmd, args);
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

  if(firstDisplay == NULL) {
    QGridLayout *layout = new QGridLayout(centralWidget);
    layout->addWidget(display);
    firstDisplay = display;
  } else if(tabs->count() == 0) {
    setCentralWidget(tabs);
    firstDisplay->setParent(tabs);
    tabs->addTab(firstDisplay,QIcon(":icon/konsole"), tr(name));
  }
  if(tabs->count() > 0) {
    int index = tabs->addTab(display,QIcon(":icon/konsole"), tr(name));
    tabs->setCurrentIndex(index);
  }
  
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
  display->setFocus(Qt::OtherFocusReason);
}

void MainWindow::finished(QObject *obj)
{
  Session *session = (Session *)obj;
  _sessionCount--;
  int index = tabs->indexOf(session->views()[0]);
  if(index >= 0) {
    tabs->removeTab(index);
  } else {
    index = 0;      // first display - no tabs yet
  }
  
  _Sessions.removeAt(index);
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
  addSession(title.toLatin1().constData(), QString(), QStringList());
}

void MainWindow::closeSession()
{
  int index = tabs->currentIndex();
  if (index < 0) {
    index = 0;      // no tabs yet
  }
  Session *session = _Sessions.at(index);
  session->close();
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
  int index = tabs->currentIndex();
  if (index < 0) {
    index = 0;      // no tabs yet
  }
  Session *sess =  _Sessions.at(index);
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
