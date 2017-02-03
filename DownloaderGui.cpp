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

// ======================================================================
// DownloaderGui.cpp
// ======================================================================
//                   This file is a part of the book 
//             "Qt 5.3 Professional programming with C++"
// ======================================================================
//  Copyright (c) 2014 by Max Schlee
//
//  Email : Max.Schlee@neonway.com
//  Blog  : http://www.maxschlee.com
//
//  Social Networks
//  ---------------
//  FaceBook : http://www.facebook.com/mschlee
//  Twitter  : http://twitter.com/Max_Schlee
//  2Look.me : http://2look.me/NW100003
//  Xing     : http://www.xing.com/profile/Max_Schlee
//  vk.com   : https://vk.com/max.schlee
// ======================================================================

#include <QtWidgets>
#include "Downloader.h"
#include "DownloaderGui.h"
#include "XmlCurrencyParser.h"

// ----------------------------------------------------------------------
DownloaderGui::DownloaderGui(QWidget* pwgt /*=0*/) : QWidget(pwgt)
{
    currencyCode = "USD";
    date = (QDate().currentDate()).addDays(1);

    QGroupBox* settingsGroup = new QGroupBox(tr("Настройки"));

    currencyCodeLineEdit = new QLineEdit(currencyCode);
    currencyCodeLineEdit->setMaxLength(3);
    currencyCodeLineEdit->setInputMask("AAA");
    currencyCodeLabel = new QLabel(tr("код валюты:"));
    dateLabel = new QLabel(tr("дата:"));
    dateLineEdit = new QLineEdit(date.toString("dd.MM.yyyy"));
    dateLineEdit->setInputMask("09.09.9999");


    QGroupBox* resultGroup = new QGroupBox(tr("Результат запроса"));
    nominalResultLabel = new QLabel("1");
    nominalLabel = new QLabel(tr("номинал:"));
    valueLabel = new QLabel(tr("курс к рублю:"));
    valueResultLabel = new QLabel;
    nameLabel = new QLabel(tr("название:"));
    nameResultLabel = new QLabel;

    QGroupBox* urlGroup = new QGroupBox(
                tr("Адрес для получения котировок на день"));
    goButton = new QPushButton(tr("Скачать"));
    QString strDownloadLink = "http://www.cbr.ru/scripts/XML_daily.asp";
    urlLineEdit = new QLineEdit(strDownloadLink);
//    urlLineEdit->setReadOnly(true);
    downProgressBar  = new QProgressBar;

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
    setLayout(pLayout);

    setWindowTitle(tr("Курс валют ЦБ РФ"));

//    xmlParserObject = new XmlCurrencyParser(this);
    xmlParserObject = new XmlCurrencyParser(this, currencyCode);
    downloaderObject = new Downloader(this);

//Connectors-------------------------------------------------------------------

    connect(goButton, SIGNAL(clicked()), this, SLOT(slotGo()));
    connect(downloaderObject, SIGNAL(downloadProgress(qint64, qint64)),
            this,  SLOT(slotDownloadProgress(qint64, qint64))
           );
    connect(downloaderObject, SIGNAL(done(const QUrl&, const QByteArray&)),
            this,  SLOT(slotDone(const QUrl&, const QByteArray&))
           );
    connect(currencyCodeLineEdit, &QLineEdit::textEdited,
            xmlParserObject, &XmlCurrencyParser::setCurrencyName
            );
    connect(currencyCodeLineEdit, &QLineEdit::textChanged,
            xmlParserObject, &XmlCurrencyParser::setCurrencyName
            );
    connect(this, &DownloaderGui::loadedXml,
            xmlParserObject, &XmlCurrencyParser::getCurrencyByCode
            );
    connect(xmlParserObject, &XmlCurrencyParser::parseSucces,
            this, &DownloaderGui::slotParseSucces
            );
}

// ----------------------------------------------------------------------
void DownloaderGui::slotGo()
{
    QDate inputDate = QDate::fromString(dateLineEdit->text(),"dd.MM.yyyy");
    if(!inputDate.isValid()
            || inputDate > (QDate().currentDate()).addDays(2)
            || inputDate < QDate(1992, 7, 1))
    {
        slotError(tr("Введена некорректная дата.\n"
                     "Диапазон дат от 01.07.1992 до завтрашней даты.\n"
                     "Исправьте, пожалуйста."));
        return;
    }

    QUrl *urlUserInput = new QUrl(urlLineEdit->text());
    if(!(urlUserInput->isValid()))
    {
        slotError(tr("Введен некорректный адрес сайта!\n"
                     "Исправьте, пожалуйста."));
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

// ----------------------------------------------------------------------
void DownloaderGui::slotDownloadProgress(qint64 nReceived, qint64 nTotal)
{
    if (nTotal <= 0) {
        slotError(tr("An error while download is occured"));
        return;
    }
    downProgressBar->setValue(100 * nReceived / nTotal);
}

// ----------------------------------------------------------------------
void DownloaderGui::slotDone(const QUrl& url, const QByteArray& ba)
{
    //TODO rewrite with lambda
    emit loadedXml(ba);
    //    QString strFileName = url.path().section('/', -1);
}

// ----------------------------------------------------------------------
void DownloaderGui::slotError()
{
    QMessageBox::critical(0,
                          tr("Error"),
                          tr("An error while download is occured")
                         );
}

// ----------------------------------------------------------------------
void DownloaderGui::slotError(const QString& errorMessage =
        "An error while download is occured")
{
    QMessageBox::critical(0,
                          tr("Error"),
                          errorMessage
                         );
}

// ----------------------------------------------------------------------
void DownloaderGui::slotParseSucces(const QString& valueParsed,
                                    const QString& nominalParsed,
                                    const QString& nameParsed)
{
    valueResultLabel->setText(valueParsed);
    nominalResultLabel->setText(nominalParsed);
    nameResultLabel->setText(nameParsed);
}
