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

//<Valute ID="R01235">
//    <NumCode>840</NumCode>
//    <CharCode>USD</CharCode>
//    <Nominal>1</Nominal>
//    <Name>Доллар США</Name>
//    <Value>30,9436</Value>
//</Valute>

#include "XmlCurrencyParser.h"

void XmlCurrencyParser::setCurrencyName(const QString& name)
{
    currencyCode = name;
}

// ----------------------------------------------------------------------
void XmlCurrencyParser::traverseNode(const QDomNode& node)
{
    if(stopParse)
        return;
    QDomNode domNode = node.firstChild();
    while(!domNode.isNull()) {
        if(domNode.isElement()) {
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()) {
                if(domElement.tagName()=="CharCode" &&
                        domElement.text()==currencyCode)
                {
                    isCurrencyCodeFound = true;
                    stopParse = true;
                    //In example next sublings are Nominal, Name, Value

                    domNode = domNode.nextSibling();
                    domElement = domNode.toElement();
                    if(domElement.tagName()=="Nominal")
                        nominal = domElement.text();

                    domNode = domNode.nextSibling();
                    domElement = domNode.toElement();
                    if(domElement.tagName()=="Name")
                        name = domElement.text();

                    domNode = domNode.nextSibling();
                    domElement = domNode.toElement();
                    if(domElement.tagName()=="Value")
                        valueToRUR = domElement.text();

                    emit parseSucces(valueToRUR, nominal, name);
                }
             }
        }
        traverseNode(domNode);
        domNode = domNode.nextSibling();
    }
}

// ----------------------------------------------------------------------
void XmlCurrencyParser::getCurrencyByCode(const QByteArray & data)
{
    QDomDocument domDoc;
    bool namespaceProcessing = false;
    QString* errorMsg = new QString("");
    int * errorLine = new int;
    int * errorColumn = new int;
    if(domDoc.setContent(data,
                         namespaceProcessing,
                         errorMsg,
                         errorLine,
                         errorColumn
                         ))
    {
        QDomElement domElement= domDoc.documentElement();
        if(!domElement.isNull()) {
            //Date check is obsolete. Commented it just in case.
//            if(domElement.tagName()=="ValCurs")
//            {
//                date = domElement.attribute("Date","");
//                emit dateFound(date);
//            }
        }
        traverseNode(domElement);
        //To enable restart
        isCurrencyCodeFound = false;
        stopParse = false;

    }
    else
    {
        emit error(*errorMsg + "in the line number " + *errorLine +
                   " in the column number " + *errorColumn);
    }
}


// ----------------------------------------------------------------------
XmlCurrencyParser::XmlCurrencyParser(QObject* pobj,
                                     const QString& curCode)
    : QObject(pobj), currencyCode(curCode)
{
    isCurrencyCodeFound = false;
    stopParse = false;
    valueToRUR = 0.0;
    currencyCode = curCode;
}

//// ----------------------------------------------------------------------
//XmlCurrencyParser::XmlCurrencyParser(QObject* pobj/*=0*/)
//    : QObject(pobj)
//{
//    isCurrencyCodeFound = false;
//    stopParse = false;
//    currencyCode = "USD";
//}

