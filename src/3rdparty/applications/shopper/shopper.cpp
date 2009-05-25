//
// C++ Implementation: shopper
//
// Description:
//
//
// Author: Lorn Potter <lpotter@trolltech.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ui_shopperbase.h"
#include "shopper.h"
#include "shopperdatabase.h"
#include "newitem.h"

#include <QtGui>

#include <QStringListModel>
#include <QAbstractItemView>
#include <QMenu>
#include <QAction>
#include <QMenuBar>

#ifdef Q_WS_QWS
#include <QSoftMenuBar>
#include <Qtopia>
#endif

ShopperBase::ShopperBase(QWidget *parent, Qt::WFlags f)
        : QWidget(parent, f)
{
    setupUi(this);

}

ShopperBase::~ShopperBase()
{

}

Shopper::Shopper( QWidget *parent, Qt::WFlags f )
: ShopperBase( parent, f )
{
    model = new QStringListModel(this);
    listView->setModel(model);
    listView->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked);
    createMenu();
    shopdb = new ShopperDatabase();
    shopdb->openDatabase();
    refreshList();
}

Shopper::~Shopper()
{
}



void Shopper::createMenu()
{
  #ifdef Q_WS_QWS
    QMenu *contextMenu = QSoftMenuBar::menuFor(this);
    contextMenu->addAction( tr("New Item"), this, SLOT( addItem()));
    contextMenu->addAction( tr("Remove Item"), this, SLOT( removeItem()));
    contextMenu->addAction( tr("Update Item"), this, SLOT( itemUpdate()));

    contextMenu->addAction( tr("Quit"), this, SLOT( close()));

    contextMenu->addSeparator();

#else
    QMenuBar *menubar;
    menubar = new QMenuBar(this);
    menubar->setObjectName(QString::fromUtf8("menubar"));

    QAction *actionItemAdd;
    actionItemAdd = new QAction("New Item", this);

    QAction *actionItemRemove;
    actionItemRemove = new QAction("Remove Item", this);

    QAction *actionQuit;
    actionQuit = new QAction("Quit", this);

    QAction *actionUpdate;
    actionUpdate = new QAction("Update", this);


    QMenu *menuFile = menubar->addMenu(tr("&File"));
    menuFile->addAction(actionItemAdd);
    menuFile->addAction(actionItemRemove);
    menuFile->addAction(actionUpdate);

    menuFile->addAction(actionQuit);

    gridLayout->addWidget(menubar, 0, 0, 1, 1);
    connect(actionItemAdd,SIGNAL(triggered()),this,SLOT(addItem()));
    connect(actionQuit,SIGNAL(triggered()),this,SLOT(close()));
    connect(actionUpdate,SIGNAL(triggered()),this,SLOT(itemUpdate()));
    connect(actionItemRemove,SIGNAL(triggered()),this,SLOT(removeItem()));

#endif
}


void Shopper::addItem()
{
    NewItem *newItemDlg;
    newItemDlg = new NewItem(this);
    newItemDlg->itemComboBox->hide();
    newItemDlg->label->hide();
    
    newItemDlg->exec();

    QString category = newItemDlg->getCategory();
    QString item = newItemDlg->getItem();
    qWarning(category.toLocal8Bit()+" "+item.toLocal8Bit());
    if(!item.isEmpty())
        shopdb->addEntry( newItemDlg->getCategory(), item);
    refreshList();
}


void Shopper::itemUpdate()
{
    if(listView->currentIndex() == QModelIndex()) {
        QMessageBox::information(this,"Shopper Update",
                                 "<p>Please select an item to update.</p>",0);

        return;
    }
    //shopdb
    NewItem *newItemDlg;
    newItemDlg = new NewItem(this);
    newItemDlg->itemComboBox->hide();
    newItemDlg->label->hide();
    //itemComboBox
    QString oldEntry = model->data(listView->currentIndex(),Qt::DisplayRole ).toString();
    newItemDlg->itemLineEdit->setText(oldEntry );
    newItemDlg->exec();

    QString category = newItemDlg->getCategory();
    QString item = newItemDlg->getItem();
    qWarning(category.toLocal8Bit()+" "+item.toLocal8Bit());

    if(!item.isEmpty())
        shopdb->updateEntry( oldEntry, item);
    refreshList();


}


void Shopper::quit()
{

}


void Shopper::refreshList()
{
    model->setStringList(shopdb->getAllEntries());
}


void Shopper::removeItem()
{
    if(listView->currentIndex() == QModelIndex()) {
        QMessageBox::information(this,"Shopper Update",
                                 "<p>Please select an item to remove.</p>",0);

        return;
    }
    shopdb->removeEntry( model->data(listView->currentIndex(),Qt::DisplayRole ).toString() );
        refreshList();
}
