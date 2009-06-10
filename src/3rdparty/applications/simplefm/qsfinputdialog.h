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

/* DESCRIPTION: as Qtopia is missing the QInputDialog, I have partially 
 * reimplemented it here so we can ask the user to insert some text 
 */

#include <QDialog>
#include <QWidget>
#include <QString>
#include <QLineEdit>

class QSFInputDialog : public QDialog
{
  Q_OBJECT

  private:
    QSFInputDialog(QWidget *parent = 0, Qt::WindowFlags f = 0);
    ~QSFInputDialog();
    void init(const QString &title,const QString &label, QWidget *input);

  public:
   static QString getText(QWidget *parent, const QString &title, 
                            const QString &label, 
                            QLineEdit::EchoMode mode = QLineEdit::Normal,
                            const QString &text = QString(), 
                            bool *ok = 0, Qt::WindowFlags f=0);
};
