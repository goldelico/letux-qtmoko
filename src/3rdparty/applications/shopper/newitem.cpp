//
// C++ Implementation: newitem
//
// Description:
//
//
// Author: Lorn Potter <lpotter@trolltech.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ui_newitembase.h"
#include "newitem.h"

NewItemBase::NewItemBase(QWidget *parent, Qt::WFlags f)
        : QDialog(parent, f)
{
    setupUi(this);

}

NewItemBase::~NewItemBase()
{

}

NewItem::NewItem( QWidget *parent, Qt::WFlags f )
: NewItemBase( parent, f )
{
}

NewItem::~NewItem()
{
}




QString NewItem::getItem()
{
    return itemLineEdit->text();
}


QString NewItem::getCategory()
{
    return itemComboBox->currentText();
}
