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
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "qsfinputdialog.h"

QSFInputDialog::QSFInputDialog(QWidget *parent, Qt::WindowFlags f)
   : QDialog(parent,f)
{
}

QSFInputDialog::~QSFInputDialog()
{
}

void QSFInputDialog::init(const QString &title,const QString &label, 
                             QWidget *input)
{
  QLabel *l = new QLabel(label);
  QVBoxLayout *vbox = new QVBoxLayout(this);
  QHBoxLayout *hbox = new QHBoxLayout();
  QPushButton *bok = new QPushButton(tr("OK"));
  QPushButton *bcancel = new QPushButton(tr("Cancel"));

  setWindowTitle(title);
  vbox->addWidget(l);
  vbox->addWidget(input);
  hbox->addWidget(bok);
  hbox->addWidget(bcancel);
  vbox->addLayout(hbox);
  connect(bok, SIGNAL(clicked(bool)), this, SLOT(accept()));
  connect(bcancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

QString QSFInputDialog::getText(QWidget *parent, const QString &title,
                       const QString &label, QLineEdit::EchoMode mode,
                       const QString &text, bool *ok, Qt::WindowFlags f)
{
  QSFInputDialog *d = new QSFInputDialog(parent,f);
  QLineEdit *e = new QLineEdit();

  
  e->setText(text);
  e->setEchoMode(mode);
  d->init(title,label,e);
  
  *ok = d->exec() == QDialog::Accepted;
  return e->text();
}
