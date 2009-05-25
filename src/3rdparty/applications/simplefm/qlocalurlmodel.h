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


#ifndef QLOCALURLMODEL_H
#define QLOCALURLMODEL_H

#include <QVariant>
#include <QStringList>
#include <QDir>

#include "qabstracturlmodel.h"

class QLocalUrlModel : public QAbstractUrlModel
{
  Q_OBJECT
  public:
    QLocalUrlModel(QString path = QDir::homePath());
    ~QLocalUrlModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, 
                        int role = Qt::DisplayRole) const;

    void setPath(QString newpath);
    QString path();
    bool isRoot();
    bool isDir(int row = 0);
    bool isExec(int row = 0);

    QString fileName(int row) const;
    QString fullFileName(int row) const;
    QString fileSize(int row) const;
    QString fileDate(int row) const;
    QString fileType(int row) const;
    QIcon fileIcon(int row) const;

    int fileCopy(QString file);
    int fileDelete(QString file);
    int fileMove(QString file);
    int fileRename(int index, QString newname);
    int createDir(QString name);

   private:
     QDir currentDir;
     QString baseDir;
     int intFileDelete(QString file);
     int intFileCopy(QString source,QString dest);
};

#endif
