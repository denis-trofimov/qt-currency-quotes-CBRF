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
        //Format of <Valuta name="Foreign Currency Market Lib">
        //<Item ID="R01235">
        //<Name>Доллар США</Name>
        //<EngName>US Dollar</EngName>
        //<Nominal>1</Nominal>
        //<ParentCode>R01235    </ParentCode>
        //</Item>

        //Creating of the data base
        QSqlQuery query;
        QString sql =
                "CREATE TABLE currency_lib ( "
                "charcode TEXT PRIMARY KEY, "
                "name       TEXT, "
                "nominal    INTEGER NOT NULL, "
                "engname    TEXT"
                ");";
        if(!query.exec(sql))
        {
            qDebug() << "Unable to create a table currency_lib in SQLite:"
                     << db.lastError();
            emit error(tr("Ошибка при создании таблицы currency_lib в БД."));
            return false;
        }
        db.commit();
    }

    if(!tablesList.contains("daily_quotes"))
    {
        //Format of <ValCurs Date="02.03.2002" name="Foreign Currency Market">
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
                "CREATE TABLE daily_quotes ( "
                "charcode TEXT, "
                "date       INTEGER, "
                "value      TEXT NOT NULL, "
                "PRIMARY KEY(charcode, date), "
                "CONSTRAINT quotes_lib FOREIGN KEY(charcode) REFERENCES "
                "currency_lib ON DELETE CASCADE ON UPDATE CASCADE"
                ");";
        if(!query.exec(sql))
        {
            qDebug() << "Unable to create a table daily_quotes in SQLite:"
                     << db.lastError();
            emit error(tr("Ошибка при создании таблицы daily_quotes в БД."));
            return false;
        }
        db.commit();
    }
    return true;
}

SqlModel::SqlModel(QWidget *parent) :
    QWidget(parent)
{
    parentWidget = parent; //to pass pointer in View
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
    QSqlQueryModel quotesModel;

    qlonglong daysJulian = date.toJulianDay();
    QString readQuotesQuery = "SELECT COUNT(*) "
                        "FROM daily_quotes "
                        "WHERE charcode = '%1' AND date = '%2';";
    quotesModel.setQuery(readQuotesQuery
                         .arg(charcode)
                         .arg(QString::number(daysJulian, 10)));

    if (quotesModel.lastError().isValid()) {
        qDebug() << quotesModel.lastError();
        return false;
    }
    int countPairCodeDate = quotesModel.data(quotesModel.index(0, 0)).toInt();
    qDebug() << "countPairCodeDate=" << countPairCodeDate;
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
                            "WHERE charcode = '%1'";
        libModel.setQuery(readLibQuery
                             .arg(charcode));

        if (libModel.lastError().isValid()) {
            qDebug() << libModel.lastError();
            return false;
        }
        int countCharCodeLib = libModel.data(libModel.index(0, 0)).toInt();
        qDebug() << "countCharCodeLib=" << countCharCodeLib;
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
            insertLib = insertLib.arg(charcode)
                    .arg(name)
                    .arg(nominal.toInt())
                    .arg(engname);
            QSqlQuery libQuery = QSqlQuery(db);
            if(!libQuery.exec(insertLib))
            {
                emit error(tr("Ошибка записи в таблицу currency_lib в БД."));
                qDebug() << "Unable to execute:"  << insertLib;
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
        insertQuotes = insertQuotes.arg(charcode)
                .arg(value)
                .arg(date.toJulianDay());

        QSqlQuery quotesQuery = QSqlQuery(db);

        if(!quotesQuery.exec(insertQuotes))
        {
            emit error(tr("Ошибка записи в таблицу daily_quotes в БД."));
            qDebug() << "Unable to execute:"  << insertQuotes;
            return false;
        }
        db.commit();
        return true;
    }
}

bool SqlModel::slotReadCurrencyValue(const QString& charcode,
                          const QDate& date, QString& value)
{
    QSqlQuery quotesQuery = QSqlQuery(db);

    qlonglong daysJulian = date.toJulianDay();
    QString readQString = "SELECT value "
                        "FROM daily_quotes "
                        "WHERE charcode = '%1' AND date = '%2';";
    readQString = readQString
                .arg(charcode)
                .arg(QString::number(daysJulian, 10));

    if(!quotesQuery.exec(readQString))
    {
        emit error(tr("Ошибка чтения таблицы daily_quotes из БД."));
        qDebug() << "Unable to execute:"  << readQString;
        return false;
    }
    QSqlRecord rec = quotesQuery.record();

    if(quotesQuery.next())
    {
        value = quotesQuery.value(rec.indexOf("value")).toString();
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
