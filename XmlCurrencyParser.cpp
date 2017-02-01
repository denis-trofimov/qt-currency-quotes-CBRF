#include "XmlCurrencyParser.h"

void XmlCurrencyParser::setCurrencyName(const QString& name)
{
    currencyName = name;
}

// ----------------------------------------------------------------------
double XmlCurrencyParser::getCurrecyValue(const QByteArray & data)
{
    QXmlStreamReader xmlsr(data);
TODO://get date
    while(!xmlsr.atEnd())
    {
        xmlsr.readNext();
        qDebug() << xmlsr.tokenString() << xmlsr.name() << xmlsr.text();
        if((xmlsr.name()=="CharCode") && (xmlsr.text()==currencyName))
        {
            nameFound = true;
            continue;
        }
        if(nameFound && (xmlsr.name()=="Value"))
        {
            valueToRUR = (xmlsr.text()).toDouble();
            emit valuefound(valueToRUR);
            break;
        }
    }
    if (xmlsr.hasError()) {
        qDebug() << "Error:" << xmlsr.errorString();
        emit error(xmlsr.errorString());
    }

    return valueToRUR;
}


// ----------------------------------------------------------------------
XmlCurrencyParser::XmlCurrencyParser(QObject* pobj/*=0*/) : QObject(pobj)
{
    nameFound = false;
    valueToRUR = 1;
}

