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


//<Valute ID="R01235">
//    <NumCode>840</NumCode>
//    <CharCode>USD</CharCode>
//    <Nominal>1</Nominal>
//    <Name>Доллар США</Name>
//    <Value>30,9436</Value>
//</Valute>

#include "XmlParser.h"

void XmlParser::slotSetCurrencyName(const QString& name)
{
    currencyCode = name;
}

// ----------------------------------------------------------------------
void XmlParser::traverseNode(const QDomNode& node)
{
    QDomNode domNode = node.firstChild();
    while(!domNode.isNull()) {
        if(domNode.isElement()) {
            QDomElement domElement = domNode.toElement();
            if(!domElement.isNull()) {
                if(domElement.tagName()=="CharCode" &&
                        domElement.text()==currencyCode)
                {
                    //In example next sublings are Nominal, Name, Value

                    domElement = domNode.nextSiblingElement("Nominal");
                    if(!domElement.isNull())
                        nominal = domElement.text();
                    else
                    {
                        domElement = domNode.previousSiblingElement("Nominal");
                        if(!domElement.isNull())
                            nominal = domElement.text();
                        else
                            *errorMsg += tr("Tag <Nominal> not found in the XML document.");
                    }

                    domElement = domNode.nextSiblingElement("Name");
                    if(!domElement.isNull())
                        name = domElement.text();
                    else
                    {
                        domElement = domNode.previousSiblingElement("Name");
                        if(!domElement.isNull())
                            name = domElement.text();
                        else
                            *errorMsg += tr("Tag <Name> not found in the XML document.");
                    }

                    domElement = domNode.nextSiblingElement("Value");
                    if(!domElement.isNull())
                        valueToRUR = domElement.text();
                    else
                    {
                        domElement = domNode.previousSiblingElement("Value");
                        if(!domElement.isNull())
                            valueToRUR = domElement.text();
                        else
                            *errorMsg += tr("Tag <Value> not found in the XML document.");
                    }

                    if(errorMsg->isEmpty())
                        emit parseSucces(valueToRUR, nominal, name);
                    else
                        emit error(*errorMsg);

                    //To stop parse constructs a null node, stops while loop
                    domNode = QDomNode();
                    continue;
                }
             }
        }
        traverseNode(domNode);
        if(!domNode.isNull())
            domNode = domNode.nextSibling();
    }
}

/*!
Parses XML file with daily quotes
\param[in] data daily quotes XML file contents in raw bytes
\param[in] code currency code like "USD"

Example of daily quotes XML file at URL:
http://www.cbr.ru/scripts/XML_daily_eng.asp?date_req=22/01/2007

\code xml
<?xml version="1.0" encoding="windows-1252" ?>
<ValCurs Date="20.01.2007" name="Foreign Currency Market">
<Valute ID="R01010">
    <NumCode>036</NumCode>
    <CharCode>AUD</CharCode>
    <Nominal>1</Nominal>
    <Name>Australian Dollar</Name>
    <Value>20,9568</Value>
</Valute>
</ValCurs>
\endcode
*/
// ----------------------------------------------------------------------
void XmlParser::slotParseDailyQuotes(const QByteArray & data, const QString& code)
{
    slotSetCurrencyName(code);
    QDomDocument domDoc;
    bool namespaceProcessing = false;
    errorMsg = new QString("");
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
    }
    else
    {
        emit error(*errorMsg + "in the line number " + *errorLine +
                   " in the column number " + *errorColumn);
    }
}

/*!
Parses two XML files at once with FCM daily quotes and library
\param[in] quotesQByteArray Daily quotes XML file contents
\param[in] libQByteArray library XML file contents
\param[out] strOutput Semicolon separated result with lib and quotes

Example of daily quotes XML file at URL:
http://www.cbr.ru/scripts/XML_daily_eng.asp?date_req=22/01/2007

\code xml
<?xml version="1.0" encoding="windows-1252" ?>
<ValCurs Date="20.01.2007" name="Foreign Currency Market">
<Valute ID="R01010">
    <NumCode>036</NumCode>
    <CharCode>AUD</CharCode>
    <Nominal>1</Nominal>
    <Name>Australian Dollar</Name>
    <Value>20,9568</Value>
</Valute>
</ValCurs>
\endcode

Example of library XML file at URL:
http://www.cbr.ru/scripts/XML_valFull.asp

\code xml
<?xml version="1.0" encoding="windows-1251" ?>
<Valuta name="Foreign Currency Market Lib">
<Item ID="R01500">
    <Name>Молдавский лей</Name>
    <EngName>Moldova Lei</EngName>
    <Nominal>10</Nominal>
    <ParentCode>R01500    </ParentCode>
    <ISO_Num_Code>498</ISO_Num_Code>
    <ISO_Char_Code>MDL</ISO_Char_Code>
</Item>
\endcode
*/
// ----------------------------------------------------------------------
void XmlParser::slotParseDailyQuotesLib(const QByteArray& libQByteArray,
                                        const QByteArray& quotesQByteArray,
                                        QString& strOutput)
{
//    QString strXQuery =
//            "declare variable $lib external;"
//            "declare variable $quotes external;"
//            "<library>"
//            "{"
//            "for $v in fn:doc($quotes)/ValCurs/Valute,"
//            "for $i in fn:doc($lib)/Valuta/Item[@ID/data() = $v/@ID/data()]"
//            "return"
//            "<item>"
//            "{"
//            "$i/ISO_Num_Code,"
//            "$i/ISO_Char_Code,"
//            "$i/EngName,"
//            "$i/Name,"
//            "$i/Nominal,"
//            "$v/Value"
//            "}"
//            "</item>"
//            "}"
//            "</library>";

    QString strXQuery =
            "declare variable $lib external;"
            "declare variable $quotes external;"
            "for $v in fn:doc($quotes)/ValCurs/Valute,"
            "for $i in fn:doc($lib)/Valuta/Item[@ID/data() = $v/@ID/data()]"
            "return"
            "fn:concat($i/ISO_Num_Code, ';', $i/ISO_Char_Code, ';', $i/EngName"
            ", ';', $i/Name, ';', $i/Nominal, ';', $v/Value, ';')";

    QBuffer libBuffer;
    libBuffer.setData(libQByteArray);
    QBuffer quotesBuffer;
    quotesBuffer.setData(quotesQByteArray);
    QXmlQuery query;
    query.bindVariable("lib", &libBuffer);
    query.bindVariable("quotes", &quotesBuffer);
    query.setQuery(strXQuery);
    if (!query.isValid()) {
        emit error(tr("The XQuery is invalid"));
        qDebug() << "The XQuery is invalid";
    }
    else
    {
        if (!query.evaluateTo(&strOutput)) {
            emit error(tr("Can't evaluate the XQuery"));
            qDebug() << "Can't evaluate the XQuery";
        }
        qDebug() << strOutput;
    }
}


