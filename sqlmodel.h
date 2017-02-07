#ifndef SQLMODEL_H
#define SQLMODEL_H

#include <QObject>
#include <QtSql>
#include <QDebug>
#include <QDate>
#include <QLocale>

class MySqlModel : public QObject
{
    Q_OBJECT

private:
    QSqlDatabase db;
//    const QString currency_lib;
//    const QString daily_quotes;


public:
//    QLocale locale;
    explicit MySqlModel(QObject *parent = 0);

signals:
    void error(const QString&);

public slots:
    bool slotCreateConnection();
    bool slotWrite(const QString& charcode,
                   const QString& value,
                   const QString& date,
                   const QString& name,
                   const QString& nominal,
                   const QString& engname = NULL
            );
    bool slotReadCurrencyValue(const QString& charcode,
                               const QString& date,
                               QString &value);
    bool slotReadLib(QSqlTableModel&);
};

#endif // SQLMODEL_H
