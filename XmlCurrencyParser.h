/* Qt app for getting currency quotes from CB RF.
 *
 * Copyright 2017 Denis Trofimov <silaradost@yandex.ru>
 * Based on modified DownloaderGui app by Max Schlee
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
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
 *
 *
 */

#ifndef XMLCURRENCYREADER_H
#define XMLCURRENCYREADER_H

#include <QString>
#include <QObject>
#include <QtXml>
#include <QDebug>

class QXmlStreamReader;

class XmlCurrencyParser : public QObject {
Q_OBJECT

private:
    void traverseNode(const QDomNode&);

public:
    QString currencyName;
    bool isCurrencyNameFound, stopParse;
    double valueToRUR;
    QString date;

    XmlCurrencyParser(QObject* pobj = 0);

signals:
    void currencyValueFound(const QString&);
    void dateFound(const QString&);
    void error(const QString&);

public slots:
    void setCurrencyName(const QString&);
    void getCurrecyValue(const QByteArray&);
};

#endif // XMLCURRENCYREADER_H
