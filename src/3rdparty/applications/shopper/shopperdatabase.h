//
// C++ Interface: shopperdatabase
//
// Description:
//
//
// Author: Lorn Potter <lpotter@trolltech.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SHOPPERDATABASE_H
#define SHOPPERDATABASE_H

#include <qobject.h>
#include <QtGui>
#include <QtSql>
#include <QSqlDatabase>
#include <QStringList>

/**
	@author Lorn Potter <lpotter@trolltech.com>
*/
class ShopperDatabase : public QObject
{
    Q_OBJECT
public:
    ShopperDatabase();

    ~ShopperDatabase();

    void addEntry(const QString &, const QString &);
    void addCategory(const QString&);
    void openDatabase();
    void updateEntry(const QString &, const QString &);
    
    QStringList getAllEntries();
    QStringList getAllEntriesFromCategory(const QString &);
    void removeEntry(const QString &item);

protected:
   QSqlDatabase db;
    
    void closeDatabase();
    void connectDatabase();
    void createTables();
        
    
};

#endif
