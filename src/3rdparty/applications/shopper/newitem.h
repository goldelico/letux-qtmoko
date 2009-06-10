//
// C++ Interface: dirTree
//
// Description:
//
//
// Author: Lorn Potter <lpotter@trolltech.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NEWITEM_H
#define NEWITEM_H

#include "ui_newitembase.h"
#include <QDialog>


/**
	@author Lorn Potter <lpotter@trolltech.com>
*/
class NewItemBase : public QDialog, public Ui::NewItemBase
{

public:
    NewItemBase(QWidget *parent=0, Qt::WFlags f = 0);

   virtual ~NewItemBase();

};

class NewItem : public NewItemBase
{
  Q_OBJECT
public:
    NewItem( QWidget *parent = 0, Qt::WFlags f = 0 );
  virtual ~NewItem();
    QString getItem();
    QString getCategory();


public slots:


protected:
    

protected slots:

private:

private slots:


};


#endif
