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
#ifndef SHOPPER_H
#define SHOPPER_H

#include "ui_shopperbase.h"
#include "shopperdatabase.h"

#include <QDialog>
#include <QAbstractItemView>
#include <QStringListModel>

class QStringListModel;
/**
	@author Lorn Potter <lpotter@trolltech.com>
*/
class ShopperBase : public QWidget, public Ui::ShopperBase
{

public:
    ShopperBase(QWidget *parent=0, Qt::WFlags f = 0);

   virtual ~ShopperBase();

};

class Shopper : public ShopperBase
{
  Q_OBJECT
public:
    Shopper( QWidget *parent = 0, Qt::WFlags f = 0 );
  virtual ~Shopper();

public slots:


protected:
QStringListModel *model;


private:
    ShopperDatabase *shopdb;
    
    void createMenu();
protected slots:
    void addItem();
    void itemUpdate();
private slots:
    void quit();
    void removeItem();
private:
    void refreshList();
};


#endif
