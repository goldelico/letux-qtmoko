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


#ifndef QABSTRACTURLMODEL_H
#define QABSTRACTURLMODEL_H

#include <QAbstractListModel>
#include <QVariant>
#include <QIcon>


class QAbstractUrlModel : public QAbstractListModel
{
  Q_OBJECT
  public:
    QAbstractUrlModel();
    ~QAbstractUrlModel();

    virtual inline void setPath(QString ) {};
    virtual inline QString path() { return ""; };
    virtual inline bool isRoot() { return false; };
    virtual inline bool isDir(int) { return false; };
    virtual inline bool isExec(int) { return false; };
    virtual inline QString fileName(int) const { return ""; };
    virtual inline QString fullFileName(int) const { return ""; };
    virtual inline QString fileSize(int) const { return ""; };
    virtual inline QString fileDate(int) const { return ""; };
    virtual inline QString fileType(int) const { return ""; };
    virtual inline QIcon fileIcon(int) const { return QIcon(); };


    virtual inline int fileCopy(QString) { return -1; };
    virtual inline int fileDelete(QString) { return -1; };
    virtual inline int fileMove(QString) { return -1; };
    virtual inline int fileRename(int,QString) { return -1; };
    virtual inline int createDir(QString) { return -1; };

  Q_SIGNALS:
    void pathChanged(QString value);
};

#endif
