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

void XmlParser::setCurrencyName(const QString& name)
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
\param[in] data XML file contents in raw bytes
\param[in] code Currency code like "USD"

Example of XML file:

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
void XmlParser::parseDailyQuotes(const QByteArray & data, const QString& code)
{
    setCurrencyName(code);
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
