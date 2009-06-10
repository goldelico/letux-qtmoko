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
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QMouseEvent>
#ifdef QTOPIA_PHONE
#include <QtopiaApplication>
#endif

#include "qurlview.h"


QUrlView::QUrlView(QWidget *parent) 
   : QListView(parent)
{
  setFocusPolicy(Qt::StrongFocus);
#ifdef QTOPIA_PHONE
  QtopiaApplication::setStylusOperation(this,QtopiaApplication::RightOnHold);
#endif

  connect(this, SIGNAL(activated(const QModelIndex&)), 
          this, SLOT(itemActivated(const QModelIndex&)));
}

QUrlView::~QUrlView() 
{
}

void QUrlView::itemActivated(const QModelIndex &index)
{
  emit listItemActivated(index.row());
}

void QUrlView::setPopUp(QMenu *menu) {
  popup = menu;
}

int QUrlView::getSelected() {
  if (currentIndex().isValid())
    return currentIndex().row();
  else
   return -1;
}

void QUrlView::mousePressEvent(QMouseEvent *ev)
{
  QListView::mousePressEvent(ev);
  if (ev->button() == Qt::RightButton && popup != 0)
    popup->popup(ev->globalPos());
}

