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

#ifndef XMLCURRENCYREADER_H
#define XMLCURRENCYREADER_H

#pragma once

#include <QString>
#include <QObject>
#include <QtXml>
#include <QDebug>


class QXmlStreamReader;

class XmlParser : public QObject {
Q_OBJECT

private:
    void traverseNode(const QDomNode&);
    QString currencyCode;
    QString valueToRUR;
    QString nominal;
    QString name;
    QString* errorMsg;

public:
    XmlParser(QObject* pobj = 0, const QString& curCode = QString("USD"));

signals:
    void error(const QString&);
    void parseSucces(const QString& valueParsed,
                         const QString& nominalParsed,
                         const QString& nameParsed);

public slots:
    void setCurrencyName(const QString&);
    void getCurrencyByCode(const QByteArray&);
};

#endif // XMLCURRENCYREADER_H
