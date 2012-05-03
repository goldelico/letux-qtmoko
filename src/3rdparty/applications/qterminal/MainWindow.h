
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui/QTabWidget>
#include <QtCore/QList>

#include "Session.h"

class MainWindow : public QMainWindow
{
 Q_OBJECT

 public:
   MainWindow(QWidget *parent=0, Qt::WindowFlags f=0);
   ~MainWindow();

//  public slots:
//    void setDocument(const QString &);
   
  private slots:
    void finished(QObject *session);
    void addSession(const char *name, QString cmd, QStringList args);
    void currentChanged(int index);
    void newSession();
    void closeSession();
    void closeAll();
    void showPreferences();

  private:
    QWidget *centralWidget;
    QTabWidget *tabs;
    TerminalDisplay *firstDisplay;
    int _sessionCount;
    QAction *acNewSession;
    QAction *acCloseSession;
    QAction *acPreferences;
    QList<Session*> _Sessions;

};

#endif
