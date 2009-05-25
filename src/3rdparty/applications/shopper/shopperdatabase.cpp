//
// C++ Implementation: shopperdatabase
//
// Description:
//
//
// Author: Lorn Potter <lpotter@trolltech.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "shopperdatabase.h"

#include <QtSql>
#include <QSqlDatabase>

#include <QSettings>
#ifdef Q_WS_QWS
#include <QSoftMenuBar>
#include <Qtopia>
#endif

ShopperDatabase::ShopperDatabase()
        : QObject()
{
}


ShopperDatabase::~ShopperDatabase()
{
}

void ShopperDatabase::openDatabase()
{
    connectDatabase();
}

void ShopperDatabase::closeDatabase()
{
    if(db.isOpen())
        db.close();

#ifdef Q_WS_QWS
    QSqlDatabase::removeDatabase(Qtopia::applicationFileName(QLatin1String("Shopper"),
        QLatin1String("shopper.sqlite")));
#else
    QSqlDatabase::removeDatabase(QLatin1String("shopper.sqlite"));
#endif

}

void ShopperDatabase::connectDatabase()
{
    if(!db.isOpen())
        if(!db.isValid()) {
            qWarning("using SQLite");
            db = QSqlDatabase::addDatabase("QSQLITE");
              // db = QSqlDatabase::addDatabase("QMYSQLDriver");
#ifdef Q_WS_QWS
            db.setDatabaseName(Qtopia::applicationFileName(QLatin1String("Shopper"),
                                                           QLatin1String("shopper.sqlite")));
#else
            db.setDatabaseName(QLatin1String("shopper.sqlite"));
#endif
            db.setUserName("shopper");
            db.setPassword("1234");

            if(!db.open()) {
                QMessageBox::critical(0, qApp->tr("Cannot open database"),
                                      qApp->tr("Unable to establish a database connection.\n"
                                               "Click Cancel to exit."),
                                      QMessageBox::Cancel,
                                      QMessageBox::NoButton);
                return;
            }

            if(!db.isValid()) {
                QMessageBox::critical(0, qApp->tr("Cannot open database"),
                                      qApp->tr("Unable to establish a database connection.\n"
                                               "Click Cancel to exit."),
                                      QMessageBox::Cancel,
                                      QMessageBox::NoButton);
                return;
            }

        }
    qWarning("%d table entry",(db.tables(QSql::Tables).count()));
    QSqlQuery xsql;
    xsql.exec(QLatin1String("PRAGMA synchronous = OFF"));
    xsql.exec(QLatin1String("PRAGMA temp_store = memory"));
    xsql.exec(QLatin1String("PRAGMA temp_store_directory = '/tmp';"));

    if(db.tables(QSql::Tables).count() < 1) {
        createTables();
    }
}

void ShopperDatabase::addEntry(const QString &cat, const QString &newEntry)
{
    QSqlQuery query;
    QString category;
    if(cat.isEmpty())
        category = "General";
    else
        category = cat;
        QString queryCommand("INSERT INTO shoppingList (category,item) VALUES (:c, :i)");

        query.prepare(queryCommand);
        query.bindValue(":c",category.simplified());
        query.bindValue(":i",newEntry.simplified());

        if(!query.exec()) {
            qWarning(query.lastError().driverText().toLocal8Bit());
            qWarning(query.lastQuery().toLocal8Bit());
            return;
            //        }
    }

}

void ShopperDatabase::addCategory(const QString&category)
{

}


void ShopperDatabase::createTables()
{
    qWarning("creating table");
    QSqlQuery query;
    query.exec("create table shoppingList(category varchar(100), item varchar(100))");
}

QStringList ShopperDatabase::getAllEntries()
{
    QStringList list;
    QSqlQuery query;
    QString queryCommand = QString("SELECT * FROM shoppingList");
    query.prepare(queryCommand);

    if(!query.exec()) {
        qWarning(query.lastError().driverText().toLocal8Bit());
        qWarning(query.lastQuery().toLocal8Bit());
        return list;
    }

    while(query.next()) {
        list <<  query.value(1).toString();
    }
    qWarning("returninstalled size %d",list.size());
    return list;
}

QStringList ShopperDatabase::getAllEntriesFromCategory(const QString & category)
{
    QStringList list;
    QSqlQuery query;
    QString queryCommand = QString("SELECT * FROM shoppingList WHERE category = ':c'");
    query.prepare(queryCommand);
    query.bindValue(":c",category.simplified());

    if(!query.exec()) {
        qWarning(query.lastError().driverText().toLocal8Bit());
        qWarning(query.lastQuery().toLocal8Bit());
        return list;
    }

    while(query.next()) {
        list  <<  query.value(0).toString();
    }
    qWarning("returninstalled size %d",list.size());
    return list;

}

void ShopperDatabase::updateEntry(const QString &oldEntry, const QString &newEntry)
{
    QSqlQuery query;
    QString queryCommand("UPDATE shoppingList SET item = :i WHERE item = :j");
    query.prepare(queryCommand);

        query.bindValue(":i",newEntry);
        query.bindValue(":j",oldEntry);

    if(!query.exec()) {
        qWarning(query.lastError().driverText().toLocal8Bit());
        qWarning(query.lastQuery().toLocal8Bit());
        return;
    }

}


void ShopperDatabase::removeEntry(const QString &item)
{
   QSqlQuery query;
    QString queryCommand("DELETE FROM shoppingList WHERE item = :i");
    query.prepare(queryCommand);

    query.bindValue(":i",item.simplified());

    if(!query.exec()) {
        qWarning(query.lastError().driverText().toLocal8Bit());
        qWarning(query.lastQuery().toLocal8Bit());
        return;
    }
}
