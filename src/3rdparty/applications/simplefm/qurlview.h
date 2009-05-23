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

#ifndef QURLVIEW_H
#define QURLVIEW_H

#include <QListView>
#include <QAbstractItemView>
#include <QAbstractItemModel>
#include <QMenu>

class QUrlView : public QListView
{
  Q_OBJECT
  public:
    //enum DisplayMode { dmList, dmDetail };

    QUrlView(QWidget *parent = 0);
    ~QUrlView();

   int getSelected();
   void setPopUp(QMenu *menu);

   void mousePressEvent(QMouseEvent *ev);

  Q_SIGNALS:
    void selectionChanged(int index);
    void listItemActivated(int index);

  private slots:
    void itemActivated(const QModelIndex &index);

  private:
    QMenu *popup;
};

#endif /* QURLVIEW_H */
