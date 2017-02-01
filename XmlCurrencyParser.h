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
//    QXmlStreamReader * pxmlsr;

public:
    QString currencyName;
    bool nameFound;
    double valueToRUR;
    QString date;

    XmlCurrencyParser(QObject* pobj = 0);

signals:
    void valuefound(double);
    void error(const QString&);

public slots:
    void setCurrencyName(const QString&);
    double getCurrecyValue(const QByteArray&);
};

#endif // XMLCURRENCYREADER_H
