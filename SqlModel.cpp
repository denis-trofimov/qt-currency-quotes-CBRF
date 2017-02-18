/* -*- coding: utf-8 -*-
* Copyright 2017 Denis Trofimov <silaradost@yandex.ru>
* All Rights Reserved.
* ========================================================================
* Author's social contacts
* ------------------------------------------------------------------------
* FaceBook | http://www.facebook.com/denis.trofimov
* Twitter  | http://twitter.com/yalyubim
* StackOverflow | http://stackoverflow.com/users/7388116/denis-trofimov
* UpWork   | https://www.upwork.com/o/profiles/users/_~01794189f31303086e/
* vk.com   | http://vk.com/denis.trofimov
* Skype    | ilikemy.self
* LinkedIn | http://www.linkedin.com/in/denis-trofimov-27713b46
* ========================================================================
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3, 29 June 2007
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
* MA 02110-1301, USA.
*/


#include "SqlModel.h"

/*!
 * \brief SqlModel::slotCreateConnection Creates connection to database
 * \return
 */
bool SqlModel::slotCreateConnection()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("qt-currency-quotes-CBRF.sqlite3");
    db.setHostName("localhost");
    db.setUserName("Denis Trofimov");
    db.setPassword("password");

    if(!db.open())
    {
        qDebug() << "Can not open local database SQLite:" << db.lastError();
        emit error(tr("Can not open local database SQLite."));
        return db.open();
    }
    return true;
}

/*!
 * \brief SqlModel::slotCreateTables Creates tables in database
 *
 * Table daily_quotes should contain data:

\code xml
    <NumCode>036</NumCode>
    <Date></Date>
    <Value>20,9568</Value>
\endcode

Table currency_lib should contain data:

\code xml
    <Name>Молдавский лей</Name>
    <EngName>Moldova Lei</EngName>
    <Nominal>10</Nominal>
    <NumCode>498</NumCode>
    <CharCode>MDL</CharCode>
\endcode
 */
void SqlModel::slotCreateTables()
{
    //Check which tables have
    QStringList tablesList = db.tables();
    if(!tablesList.contains("currency_lib"))
    {
        //Creating of the data base
        QSqlQuery query;
        QString sql =
                "CREATE TABLE currency_lib ("
                "numcode    INTEGER PRIMARY KEY"
                ", nominal  INTEGER NOT NULL"
                ", charcode TEXT  NOT NULL"
                ", name     TEXT"
                ", engname  TEXT"
                ");";
        if(!query.exec(sql))
        {
            qDebug() << "Unable to create a table currency_lib in the database:"
                     << db.lastError();
            emit error(tr("Unable to create a table currency_lib in the database."));
            return false;
        }
        db.commit();
    }

    if(!tablesList.contains("daily_quotes"))
    {
        //Creating of the data base
        QSqlQuery query;
        QString sql =
                "CREATE TABLE daily_quotes ( "
                "numcode    INTEGER PRIMARY KEY"
                ", date     TEXT NOT NULL"
                ", value    TEXT NOT NULL"
                ", PRIMARY KEY(numcode, date)"
                ", CONSTRAINT quotes_lib FOREIGN KEY(numcode) REFERENCES"
                " currency_lib ON DELETE CASCADE ON UPDATE CASCADE"
                ");";
        if(!query.exec(sql))
        {
            qDebug() << "Unable to create a table daily_quotes in the database:"
                     << db.lastError();
            emit error(tr("Unable to create a table daily_quotes in the database."));
            return false;
        }
        db.commit();
    }
}

SqlModel::SqlModel(QWidget *parent) :
    QWidget(parent)
{
    parentWidget = parent; //to pass pointer in View
//    QLocale::setDefault(QLocale::Russian);
//    if(slotCreateConnection())
//        qDebug() << "Database and tables opened";
}

/*!
 * \brief SqlModel::slotWrite
 * \param charcode
 * \param value
 * \param date
 * \param name
 * \param nominal
 * \param numcode
 * \return
 * TODO: rusian name or engname depends of locale.
 */
bool SqlModel::slotWrite(const QString& charcode,
                         const QString& value,
                         const QDate& date,
                         const QString& name,
                         const uint nominal,
                         const uint numcode)
{
    //                    const QString& engname
    //Write if not have same pair <numcode, value> in daily_quotes
    QSqlQueryModel quotesModel;

//    qlonglong daysJulian = date.toJulianDay();
    QString readQuotesQuery = "SELECT COUNT(*) "
                        "FROM daily_quotes "
                        "WHERE numcode = '%1' AND date = '%2';";
    quotesModel.setQuery(readQuotesQuery
                         .arg(numcode)
                         .arg(date.toString("yyyy/MM/dd"));
//                         .arg(QString::number(daysJulian)));

    if (quotesModel.lastError().isValid()) {
        qDebug() << quotesModel.lastError();
        return false;
    }
    int countPairCodeDate = quotesModel.data(quotesModel.index(0, 0)).toInt();

    if(countPairCodeDate != 0)
    {
        //not writing at all
        return false;
    }
    else
    {
        //What if currency in lib already?
        QSqlQueryModel libModel;

        QString readLibQuery = "SELECT COUNT(*) "
                            "FROM currency_lib "
                            "WHERE numcode = '%1'";
        libModel.setQuery(readLibQuery
                             .arg(numcode);

        if (libModel.lastError().isValid()) {
            qDebug() << libModel.lastError();
            return false;
        }
        int countCharCodeLib = libModel.data(libModel.index(0, 0)).toInt();

        if(countCharCodeLib == 0)
        {
            //Should write new record to currency_lib first in case of foreign
            //key check (in future)
            QString insertLib = "INSERT INTO currency_lib "
                                "("
                                "numcode"
                                ", charcode"
                                ", nominal"
                                ", name"
                                ")"
                                " VALUES ('%1', '%2', '%3', '%4');";
            insertLib = insertLib
                    .arg(numcode)
                    .arg(nominal)
                    .arg(charcode)
                    .arg(name)
                    ;

            QSqlQuery libQuery = QSqlQuery(db);
            if(!libQuery.exec(insertLib))
            {
                emit error(tr("Can not write to currency_lib table in the database."));
                qDebug() << "Unable to execute:"  << insertLib;
                return false;
            }

        }

        //Should write new record to daily_quotes

        QString insertQuotes = "INSERT INTO daily_quotes "
                               "("
                               "numcode"
                               ", value"
                               ", date"
                               ")"
                               " VALUES ('%1', '%2', '%3');";
        insertQuotes = insertQuotes
                .arg(numcode)
                .arg(value)
                .arg(date.toString("yyyy/MM/dd"));
//                .arg(date.toJulianDay());

        QSqlQuery quotesQuery = QSqlQuery(db);

        if(!quotesQuery.exec(insertQuotes))
        {
            emit error(tr("Can not write to daily_quotes table in the database."));
            qDebug() << "Unable to execute:"  << insertQuotes;
            return false;
        }
        db.commit();
        return true;
    }
}

bool SqlModel::slotReadCurrencyValue(const QString& charcode,
                                     const QDate& date,
                                     QString& value,
                                     QString& nominal,
                                     QString& name)
{
    QSqlQuery quotesQuery = QSqlQuery(db);

//    qlonglong daysJulian = date.toJulianDay();
    QString readQString = "SELECT value, nominal, name "
                          "FROM daily_quotes as dq, currency_lib as cl "
                          "WHERE cl.charcode = '%1' AND dq.date = '%2' "
                          "AND dq.numcode = cl.numcode;";
    readQString = readQString
            .arg(charcode)
            .arg(date.toString("yyyy/MM/dd"));
//                .arg(QString::number(daysJulian, 10));

    if(!quotesQuery.exec(readQString))
    {
        emit error(tr("Failed to read daily_quotes table from the database."));
        qDebug() << "Unable to execute:"  << readQString;
        return false;
    }
    QSqlRecord rec = quotesQuery.record();

    if(quotesQuery.next())
    {
        value = quotesQuery.value(rec.indexOf("value")).toString();
        nominal = quotesQuery.value(rec.indexOf("nominal")).toString();
        name = quotesQuery.value(rec.indexOf("name")).toString();
        return true;
    }
    else
        return false;
}

void SqlModel::slotView()
{
    QTableView* quotesView = new QTableView;
    QSqlTableModel* quotesModel = new QSqlTableModel(this, db);

    quotesModel->setTable("daily_quotes");
    quotesModel->select();
    quotesModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    quotesView->setModel(quotesModel);
//    quotesView->resize(450, 100);
    quotesView->show();

    QTableView* libView = new QTableView;
    QSqlTableModel* libModel = new QSqlTableModel(this, db);

    libModel->setTable("currency_lib");
    libModel->select();
    libModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    libView->setModel(libModel);
//    libView->resize(450, 100);
    libView->show();
}
