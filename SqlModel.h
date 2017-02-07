#ifndef SQLMODEL_H
#define SQLMODEL_H

#include <QObject>
#include <QtSql>
#include <QDebug>
#include <QDate>
#include <QLocale>
#include <QtWidgets>

class SqlModel : public QObject
{
    Q_OBJECT

private:
    QSqlDatabase db;
    QObject *parentWidget;
//    const QString currency_lib;
//    const QString daily_quotes;


public:
//    QLocale locale;
    explicit SqlModel(QObject *parent = 0);

signals:
    void error(const QString&);

public slots:
    bool slotCreateConnection();
    bool slotWrite(const QString& charcode,
                   const QString& value,
                   const QDate &date,
                   const QString& name,
                   const QString& nominal,
                   const QString& engname = NULL
            );
    bool slotReadCurrencyValue(const QString& charcode,
                               const QDate &date,
                               QString &value);
    void slotView();
    //bool slotReadLib(QSqlTableModel&) = 0;
};

#endif // SQLMODEL_H
