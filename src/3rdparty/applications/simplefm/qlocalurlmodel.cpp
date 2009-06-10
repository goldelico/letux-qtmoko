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

#include <QDebug>
#include <QStringList>
#include <QAbstractListModel>
#include <QModelIndex>
#include <QVariant>
#include <QFileInfo>
#include <QIcon>
#include <QDateTime>

#include "qlocalurlmodel.h"


QLocalUrlModel::QLocalUrlModel(QString path)
       : QAbstractUrlModel()
{
  currentDir.setSorting(QDir::DirsFirst);
  currentDir.setFilter(currentDir.filter() | QDir::NoDotAndDotDot);
  baseDir=path;
  setPath(path);
}

QLocalUrlModel::~QLocalUrlModel()
{
}

int QLocalUrlModel::rowCount(const QModelIndex &) const
{
  return currentDir.count();
}

QVariant QLocalUrlModel::data(const QModelIndex &index, int role) const
{
 if (!index.isValid())
   return QVariant();

 const int row = index.row();
 if ((unsigned int)index.row() >= currentDir.count())
   return QVariant();

 switch (role) {
   case Qt::DisplayRole:
     switch (index.column()) {
       case 0: return fileName(row); break;
       case 1: return fileSize(row); break;
       case 2: return fileDate(row); break;
       case 3: return fileType(row); break;
     }
     break;
   case Qt::DecorationRole:
     if (index.column()==0) 
       return fileIcon(index.row());
     break;
   case Qt::EditRole:
   case Qt::ToolTipRole:
   case Qt::StatusTipRole:
   case Qt::WhatsThisRole:
   case Qt::SizeHintRole:
      return QVariant();
     break;
   default:
      return QVariant();
 }
}

QString QLocalUrlModel::fileName(int row) const
{
  return currentDir[row];
}

QString QLocalUrlModel::fullFileName(int row) const
{
  return currentDir.absoluteFilePath(currentDir[row]);
}

QString QLocalUrlModel::fileSize(int row) const
{
  QFileInfo fi(currentDir,currentDir[row]);
  return QString::number(fi.size());
}

QString QLocalUrlModel::fileDate(int row) const
{
  QFileInfo fi(currentDir,currentDir[row]);
  return fi.lastModified().toString();
}

QString QLocalUrlModel::fileType(int) const
{
  /*QFileInfo fi(currentDir,currentDir[row]);
  return fi.lastModified().toString();*/
  return "";
}

QIcon QLocalUrlModel::fileIcon(const int row) const
{
  QFileInfo fi(currentDir,currentDir[row]);
  if (fi.isDir()) {
    return QIcon(":icon/folder");
  } else if (fi.isExecutable()) {
    return QIcon(":icon/exec");
  } else if (fi.isSymLink()) {
    return QIcon(":icon/symlink");
  } else {
    return QIcon(":icon/file");
  }
  return QIcon();
}

int QLocalUrlModel::columnCount(const QModelIndex &) const
{
  return 4;
}


QVariant QLocalUrlModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const
{
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
      case 0: return tr("File"); break;
      case 1: return tr("Size"); break;
      case 2: return tr("Date"); break;
      case 3: return tr("Type"); break;
    }
    return QVariant();
  } else {
    //return QString("Row %1").arg(section);
    return QVariant();
  }
}

void QLocalUrlModel::setPath(QString newpath)
{
  if ( newpath != path()  && currentDir.exists(newpath) ) {
    emit layoutAboutToBeChanged();
    currentDir.cd(newpath);
    emit pathChanged(newpath);
    emit layoutChanged();
  }
}

QString QLocalUrlModel::path()
{
  return currentDir.canonicalPath();
}

bool QLocalUrlModel::isRoot() 
{
  return (path() == baseDir);
}

bool QLocalUrlModel::isDir(int row)
{
  QFileInfo fi(currentDir,currentDir[row]);
 
  return fi.isDir();
}

bool QLocalUrlModel::isExec(int row)
{
  QFileInfo fi(currentDir,currentDir[row]);

  return fi.isExecutable();
}

int QLocalUrlModel::intFileCopy(QString source,QString dest)
{
  QFileInfo f(source);
qWarning()<<"copy"<<source<<dest;
  if (f.isDir()) {
    /* recursively copy this should include the permissions */
qWarning()<<"is dir";
    currentDir.mkdir(dest);
    QDir d(dest);
    QDir s(source);
    d.setFilter(currentDir.filter() | QDir::NoDotAndDotDot);
    s.setFilter(currentDir.filter() | QDir::NoDotAndDotDot);
    for (unsigned int i=0; i<s.count(); i++) {
       if (intFileCopy(s.absoluteFilePath(s[i]),d.absoluteFilePath(s[i])) != 0) { 
          d.refresh();
          while (d.count()>0) {
          if (intFileDelete(d.absolutePath()) != 0 )
            return -2;
          }
          return -1;
       }
    }      
    return 0; 
  } else {
qWarning()<<"is file";
    QFile f(source);
    if (f.copy(dest)) {
      return 0;
    } else {
      return -3;
    }
  }
}

int QLocalUrlModel::fileCopy(QString file)
{
  // FIXME: should check for the existence
  int r = intFileCopy(file,
            path()+QDir::separator()+QFileInfo(file).fileName()); 
  if (r == 0){
    emit layoutAboutToBeChanged();
    currentDir.refresh();
    emit layoutChanged();
    return 0;
  } else
    return -1;
}

int QLocalUrlModel::intFileDelete(QString file)
{
  if (QFileInfo(file).isDir()) {
    /* recursively remove folder */
    QString savepath = path();
    currentDir.cd(file);
    int r = 0;
    while (currentDir.count()>0) {
      r=intFileDelete(fullFileName(0));
      if (r!=0) {
        currentDir.cd(savepath);
        return r;
      } else {
        currentDir.refresh();
      }
    }
    currentDir.cd(savepath);
    if (currentDir.rmdir(file)) 
      return 0;  
    else
      return -2;
  } else if (QFile(file).remove()) {
    return 0;
  } else
    return -1;
}

int QLocalUrlModel::fileDelete(QString file)
{
  emit layoutAboutToBeChanged();
  int result = intFileDelete(file);
  currentDir.refresh();
  emit layoutChanged();
  return result;
}

int QLocalUrlModel::fileMove(QString file)
{
  int result = 0;
  QFileInfo fi(file) ;
  if (fi.isDir()) {
   QDir d(file);
   if (d.rename(file,path()+QDir::separator()+d.dirName()))
     result = 0;
   else
     result = -1;
  } else {
   QFile f(file);
   if (f.rename(path()+QDir::separator()+f.fileName()))
     result = 0;
   else
     result = -2;
  }
  if (result == 0) {
    emit layoutAboutToBeChanged();
    currentDir.refresh();
    emit layoutChanged();
  }
  return result;
}

int QLocalUrlModel::createDir(QString name)
{
  if (currentDir.mkdir(name)) {
    emit layoutAboutToBeChanged();
    currentDir.refresh();
    emit layoutChanged();
    return 0;
  }
  return -1;
}


int QLocalUrlModel::fileRename(int index, QString newname)
{
  if (QFile(fullFileName(index)).rename(path()+QDir::separator()+newname)) {
    emit layoutAboutToBeChanged();
    currentDir.refresh();
    emit layoutChanged();
    return 0;
  }
  return -1;
}
