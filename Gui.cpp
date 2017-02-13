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


#include "Gui.h"
#include <assert.h>

// ----------------------------------------------------------------------
Gui::Gui(QWidget* pwgt /*=0*/) : QWidget(pwgt)
{
    currencyCode = "USD";
    date = (QDate().currentDate()).addDays(1);

    QGroupBox* settingsGroup = new QGroupBox(tr("Settings"));

    currencyCodeLineEdit = new QLineEdit(currencyCode);
    currencyCodeLineEdit->setMaxLength(3);
    currencyCodeLineEdit->setInputMask("AAA");
    currencyCodeLabel = new QLabel(tr("currency code:"));
    dateLabel = new QLabel(tr("date:"));
    dateLineEdit = new QLineEdit(date.toString("dd.MM.yyyy"));
    dateLineEdit->setInputMask("09.09.9999");


    QGroupBox* resultGroup = new QGroupBox(tr("Query result"));
    nominalResultLabel = new QLabel("1");
    nominalLabel = new QLabel(tr("the denomination of the currency:"));
    valueLabel = new QLabel(tr("exchange rate to ruble (RUR):"));
    valueResultLabel = new QLabel;
    nameLabel = new QLabel(tr("the name of the currency:"));
    nameResultLabel = new QLabel;


    QGroupBox* urlGroup = new QGroupBox(
                tr("URL address to get daily quotes"));
    goButton = new QPushButton(tr("Update"));
    QString strDownloadLink = "http://www.cbr.ru/scripts/XML_daily.asp";
    urlLineEdit = new QLineEdit(strDownloadLink);
//    urlLineEdit->setReadOnly(true);
    downProgressBar  = new QProgressBar;

    QPushButton* viewButton = new QPushButton(tr("View database storage"));

    QFormLayout* settingsLayout = new QFormLayout;
    settingsLayout->addRow(currencyCodeLabel, currencyCodeLineEdit);
    settingsLayout->addRow(dateLabel, dateLineEdit);
    settingsGroup->setLayout(settingsLayout);

    QFormLayout* resultLayout = new QFormLayout;
    resultLayout->addRow(nameLabel, nameResultLabel);
    resultLayout->addRow(nominalLabel, nominalResultLabel);
    resultLayout->addRow(valueLabel, valueResultLabel);
    resultGroup->setLayout(resultLayout);

    QGridLayout* urlLayout = new QGridLayout;
    urlLayout->addWidget(urlLineEdit, 0, 0);
    urlLayout->addWidget(goButton, 0, 1);
    urlLayout->addWidget(downProgressBar, 1, 0, 1, 1);
    urlGroup->setLayout(urlLayout);

    QVBoxLayout* pLayout = new QVBoxLayout;
    pLayout->addWidget(settingsGroup);
    pLayout->addWidget(urlGroup);
    pLayout->addWidget(resultGroup);
    pLayout->addWidget(viewButton);
    setLayout(pLayout);

    setWindowTitle(tr("Exchange rate of the Central Bank of the Russian Federation"));
    //this->show();

    //Connectors after constructors--------------------------------------------
    connect(goButton, SIGNAL(clicked()), this, SLOT(slotGo()));

    sqlModelObject = new SqlModel(this);
    connect(sqlModelObject, &SqlModel::error,
            this, &Gui::slotError);
    bool isConnected = sqlModelObject->slotCreateConnection();
//        qDebug() << "Database and tables opened";

    connect(viewButton, &QPushButton::clicked,
            sqlModelObject, &SqlModel::slotView);


    downloaderObject = new Downloader(this);
    connect(downloaderObject, SIGNAL(downloadProgress(qint64, qint64)),
            this,  SLOT(slotDownloadProgress(qint64, qint64))
           );
    connect(downloaderObject, SIGNAL(done(const QUrl&, const QByteArray&)),
            this,  SLOT(slotDone(const QUrl&, const QByteArray&))
           );
    connect(downloaderObject, &Downloader::error,
            this, &Gui::slotError);

    xmlParserObject = new XmlParser(this, currencyCode);
    connect(currencyCodeLineEdit, &QLineEdit::textEdited,
            xmlParserObject, &XmlParser::setCurrencyName
            );
    connect(currencyCodeLineEdit, &QLineEdit::textChanged,
            xmlParserObject, &XmlParser::setCurrencyName
            );
    connect(this, &Gui::loadedXml,
            xmlParserObject, &XmlParser::getCurrencyByCode
            );
    connect(xmlParserObject, &XmlParser::parseSucces,
            this, &Gui::slotParseSucces
            );
    connect(xmlParserObject, &XmlParser::error,
            this, &Gui::slotError);
}

// ----------------------------------------------------------------------
void Gui::slotGo()
{
    downProgressBar->setValue(0);
    QDate inputDate = QDate::fromString(dateLineEdit->text(),"dd.MM.yyyy");
    if(!inputDate.isValid()
            || inputDate > (QDate().currentDate()).addDays(1)
            || inputDate < QDate(1992, 7, 1))
    {
        slotError(tr("Date is invalid. The range of valid dates is from "
                     "01.07.1992 until tomorrow's date. Correct, please."));
        return;
    }

    //First look in db, later look in XML.
    if(sqlModelObject->slotReadCurrencyValue(currencyCodeLineEdit->text(),
                                             inputDate,
                                             value,
                                             nominal,
                                             name
                                             ))
    {
        valueResultLabel->setText(value);
        nominalResultLabel->setText(nominal);
        nameResultLabel->setText(name);
    }

    else
    {
        QUrl *urlUserInput = new QUrl(urlLineEdit->text());
        if(!(urlUserInput->isValid()))
        {
            slotError(tr("You have entered an incorrect site URL! "
                         "Correct, please."));
            return;
        }

        if(date != inputDate)
        {
            date = inputDate;
            QUrlQuery *dateUrlQuery = new QUrlQuery(urlUserInput->query());
            //Insert new date to QUrlQuery
            if(dateUrlQuery->hasQueryItem("date_req"))
                dateUrlQuery->removeQueryItem("date_req");
            dateUrlQuery->addQueryItem("date_req", inputDate.toString("dd/MM/yyyy"));
            urlUserInput->setQuery(*dateUrlQuery);
            urlLineEdit->setText(urlUserInput->toString());
        }
        downloaderObject->download(*urlUserInput);
    }


}

// ----------------------------------------------------------------------
void Gui::slotDownloadProgress(qint64 nReceived, qint64 nTotal)
{
    if (nTotal <= 0) {
        slotError(tr("An error while download is occured!"));
        return;
    }
    downProgressBar->setValue(100 * nReceived / nTotal);
}

// ----------------------------------------------------------------------
void Gui::slotDone(const QUrl& url, const QByteArray& ba)
{
    //TODO rewrite with lambda
    emit loadedXml(ba);
    //    QString strFileName = url.path().section('/', -1);
}

// ----------------------------------------------------------------------
//void Gui::slotError()
//{
//    QMessageBox::critical(0,
//                          tr("Error!"),
//                          tr("An error while download is occured!")
//                         );
//}

// ----------------------------------------------------------------------
void Gui::slotError(const QString& errorMessage)
{
    QMessageBox::critical(0,
                          tr("Error!"),
                          errorMessage
                         );
}

// ----------------------------------------------------------------------
void Gui::slotParseSucces(const QString& valueParsed,
                                    const QString& nominalParsed,
                                    const QString& nameParsed)
{
    valueResultLabel->setText(valueParsed);
    nominalResultLabel->setText(nominalParsed);
    nameResultLabel->setText(nameParsed);

    sqlModelObject->slotWrite(currencyCodeLineEdit->text(),
                              valueParsed,
                              date,
                              nameParsed,
                              nominalParsed);
}
