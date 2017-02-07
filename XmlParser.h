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
