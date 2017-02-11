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

// ----------------------------------------------------------------------
void XmlParser::getCurrencyByCode(const QByteArray & data)
{
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

// ----------------------------------------------------------------------
XmlParser::XmlParser(QObject* pobj,
                     const QString& curCode)
    : QObject(pobj), currencyCode(curCode)
{
    currencyCode = curCode;
}
