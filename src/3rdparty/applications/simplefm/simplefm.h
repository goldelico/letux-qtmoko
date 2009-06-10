/*
 *  Alessandro Briosi <alex@briosix.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */


#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QMainWindow>
#include <QComboBox>
#include <QAction>
#include <QList>
#include <QLabel>
#include <QTabWidget>
#include <QDir>
#include <QIcon>

#include "qabstracturlmodel.h"
#include "qurlview.h"

class SimpleFM : public QMainWindow 
{
  Q_OBJECT
  public:
    SimpleFM(QWidget *parent = 0, Qt::WindowFlags fl = 0);
    ~SimpleFM();

  private slots:
    void cmbPathChanged(const QString path);
    void pathChanged(QString value);
    void listItemActivated(int index);
    void currentViewChanged(int index);

    void fileOpen();
    void newDirectory();
    void pathOpen();
    void fileRename();
    void sfPreferences();
    void fileCopy();
    void fileCut();
    void filePaste();
    void fileDelete();
    void fileProperties();
    void parentDirectory();

  private:
    QWidget *setupUI();
    QUrlView *currentView;
    QAbstractUrlModel *currentModel;
    QList<QUrlView *> views;
    QList<QAbstractUrlModel *> models;
    QLabel *lbPath;
    QComboBox *cmbPath;
    QTabWidget *tbSessions;
    QString clipboard;
    int clipboardaction; /* 0 = copy, 1 = move */
  
    QAction *acFileOpen;
    QAction *acNewDirectory;
    QAction *acPathOpen;
    QAction *acFileRename;
    QAction *acSFPreferences;
    QAction *acFileCopy;
    QAction *acFileCut;
    QAction *acFilePaste;
    QAction *acFileDelete;
    QAction *acFileProperties;
    QAction *acParentDirectory;
    QMenu *popupM;

   void addView(QString name, QString plugin = "local", 
                    QString path = QDir::homePath(), QIcon icon = QIcon());

};

#endif /* FILEMANAGER_H */
