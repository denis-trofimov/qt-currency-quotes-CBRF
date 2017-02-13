/* -*- coding: utf-8 -*-
* Copyright 2017 Denis Trofimov <silaradost@yandex.ru>
* All Rights Reserved.
*
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


#ifndef SQLMODEL_H
#define SQLMODEL_H

#include <QObject>
#include <QtSql>
#include <QDebug>
#include <QDate>
#include <QLocale>
#include <QtWidgets>

class SqlModel : public QWidget
{
    Q_OBJECT

private:
    QSqlDatabase db;
    QObject *parentWidget;
//    const QString currency_lib;
//    const QString daily_quotes;


public:
//    QLocale locale;
    explicit SqlModel(QWidget *parent = 0);

signals:
    void error(const QString&);

public slots:
    bool slotCreateConnection();
    bool slotWrite(const QString& charcode,
                   const QString& value,
                   const QDate &date,
                   const QString& name,
                   const QString& nominal);
    bool slotReadCurrencyValue(const QString& charcode,
                               const QDate &date,
                               QString &value,
                               QString &nominal,
                               QString &name);
    void slotView();
    //bool slotReadLib(QSqlTableModel&) = 0;
};

#endif // SQLMODEL_H
