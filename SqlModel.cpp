#include "SqlModel.h"

//Connection to SQL database
bool SqlModel::slotCreateConnection()
{

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("qtccCBRF.sqlite3");
    db.setHostName("localhost");
    db.setUserName("Denis Trofimov");
    db.setPassword("password");


    if(!db.open())
    {
        qDebug() << "Cannot open SQLite database:" << db.lastError();
        emit error(tr("Ошибка при открытии локальной БД SQLite."));
        return db.open();
    }

    //Check which tables have

    QStringList tablesList = db.tables();

    if(!tablesList.contains("currency_lib"))
    {
        //<Item ID="R01235">
        //<Name>Доллар США</Name>
        //<EngName>US Dollar</EngName>
        //<Nominal>1</Nominal>
        //<ParentCode>R01235    </ParentCode>
        //</Item>

        //Creating of the data base
        QSqlQuery query;
        QString sql =
                "CREATE DATABASE currency_lib ( "
                "charcode TEXT PRIMARY KEY NOT NULL, "
                "name       TEXT, "
                "nominal    INTEGER NOT NULL, "
                "engname    TEXT"
                ");";
        if(!query.exec(sql))
        {
            qDebug() << "Unable to create a table currency_lib in SQLite:"
                         << db.lastError();
            emit error(tr("Ошибка при создании таблицы currency_lib в БД SQLite."));
            return false;
        }
    }

    if(!tablesList.contains("daily_quotes"))
    {
        //<Valute ID="R01235">
        //	<NumCode>840</NumCode>
        //	<CharCode>USD</CharCode>
        //	<Nominal>1</Nominal>
        //	<Name>Доллар США</Name>
        //	<Value>30,9436</Value>
        //</Valute>

        //Creating of the data base
        QSqlQuery query;
        QString sql =
                "CREATE DATABASE daily_quotes ( "
                "charcode TEXT PRIMARY KEY NOT NULL, "
                "value      TEXT NOT NULL, "
                "date       INTEGER PRIMARY KEY NOT NULL "
                ");";
        if(!query.exec(sql))
        {
            qDebug() << "Unable to create a table daily_quotes in SQLite:"
                         << db.lastError();
            emit error(tr("Ошибка при создании таблицы daily_quotes в БД SQLite."));
            return false;
        }
    }
    return true;
}

SqlModel::SqlModel(QObject *parent) :
    QObject(parent)
{
    QLocale::setDefault(QLocale::Russian);
//    if(slotCreateConnection())
//        qDebug() << "Database and tables opened";
}


bool SqlModel::slotWrite(const QString& charcode,
                    const QString& value,
                    const QDate& date,
                    const QString& name,
                    const QString& nominal,
                    const QString& engname
                    )
{
    //Write if not have same pair <charcode, value> in daily_quotes

    qlonglong daysJulian = date.toJulianDay();
    QSqlQueryModel quotesModel;
    quotesModel.setQuery(QString(
                             "SELECT COUNT(*) "
                             "FROM daily_quotes "
                             "WHERE charcode = " + charcode + " AND date = " +
                             QString::number(daysJulian, 10) + ";"
                             ));
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
        libModel.setQuery(QString(
                              "SELECT COUNT(*) "
                              "FROM currency_lib "
                              "WHERE charcode = " + charcode + ";"
                              ));

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
                                "charcode"
                                ", name"
                                ", nominal"
                                ", engname"
                                ")"
                                " VALUES ('%1', '%2', '%3', '%4');";
            QString insertLibRecord = insertLib.arg(charcode)
                    .arg(name)
                    .arg(nominal.toInt())
                    .arg(engname);
            QSqlQuery libQuery;
            if(!libQuery.exec(insertLibRecord))
            {
                qDebug() << "Unable to make insert opeation insertLibRecord";
                return false;
            }
        }

        //Should write new record to daily_quotes

        QString insertQuotes = "INSERT INTO daily_quotes "
                               "("
                               "charcode"
                               ", value"
                               ", date"
                               ")"
                               " VALUES ('%1', '%2', '%3');";
        QString insertQuotesRecord = insertQuotes.arg(charcode)
                .arg(value)
                .arg(date.toJulianDay());
        QSqlQuery quotesQuery;
        if(!quotesQuery.exec(insertQuotesRecord))
        {
            qDebug() << "Unable to make insert opeation insertLibRecord";
            return false;
        }
        return true;
    }
}

bool SqlModel::slotReadCurrencyValue(const QString& charcode,
                          const QDate& date, QString& value)
{
    qlonglong daysJulian = date.toJulianDay();

    QSqlQueryModel quotesModel;
    quotesModel.setQuery(QString(
                             "SELECT charcode, value, date "
                             "FROM daily_quotes "
                             "WHERE charcode = " + charcode + " AND date = " +
                             QString::number(daysJulian, 10) + ";"
                             ));

    if (quotesModel.lastError().isValid()) {
        qDebug() << quotesModel.lastError();
        return false;
    }
    if(quotesModel.record(1).isEmpty())
        qDebug() << "QSqlQueryModel record(1) is empty.";
        return false;
    value = quotesModel.record(1).value("value").toString();
    qDebug() << "value = " << value;
    return true;
}
