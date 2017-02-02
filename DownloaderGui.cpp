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
    downloaderObject = new Downloader(this);
    this->setWindowTitle(tr("Курс валют ЦБ РФ"));

    QGroupBox * settingsGroup = new QGroupBox(tr("Настройки"));

    currencyValueLabel = new QLabel();
    xmlParserObject = new XmlCurrencyParser(this);
    currencyNameLineEdit = new QLineEdit;
    currencyLabel = new QLabel(tr("код валюты:"));
    valueLable = new QLabel(tr("курс к рублю:"));
    dateLable = new QLabel(tr("дата:"));
    dateLineEdit = new QLineEdit;

    QGroupBox* urlGroup = new QGroupBox(
                tr("Адрес для получения котировок на заданный день"));
    urlLineEdit = new QLineEdit;
    goButton = new QPushButton(tr("Скачать"));



    downProgressBar  = new QProgressBar;


    QGridLayout *settingsLayout = new QGridLayout;
    settingsLayout->addWidget(currencyLabel, 0, 0);
    settingsLayout->addWidget(currencyNameLineEdit, 0, 1);
    settingsLayout->addWidget(currencyValueLabel,1 , 1);
    settingsLayout->addWidget(valueLable, 1, 0);
    settingsLayout->addWidget(dateLable, 2, 0);
    settingsLayout->addWidget(dateLineEdit, 2, 1);
    settingsGroup->setLayout(settingsLayout);

    QGridLayout* urlLayout = new QGridLayout;
    urlLayout->addWidget(urlLineEdit, 0, 0);
    urlLayout->addWidget(goButton, 0, 1);
    urlGroup->setLayout(urlLayout);

    QGridLayout* pLayout = new QGridLayout;
    pLayout->addWidget(settingsGroup, 0, 0);
    pLayout->addWidget(urlGroup, 1, 0);
    pLayout->addWidget(downProgressBar, 2, 0, 1, 1);
    setLayout(pLayout);

    QString strDownloadLink =
            "http://www.cbr.ru/scripts/XML_daily.asp";
    urlLineEdit->setText(strDownloadLink);
    currencyNameLineEdit->setText(tr("USD"));
    currencyNameLineEdit->setMaxLength(3);
    currencyNameLineEdit->setInputMask("AAA");

    //TODO check first Date="01.07.1992"
    dateLineEdit->setInputMask("00.00.0000");
    dateLineEdit->setText("00000000");
    dateLineEdit->setCursorPosition(0);
    dateLineEdit->setReadOnly(true);

//Connectors----------------------------------------------------------
    connect(goButton, SIGNAL(clicked()), SLOT(slotGo()));
    connect(downloaderObject, SIGNAL(downloadProgress(qint64, qint64)),
            this,  SLOT(slotDownloadProgress(qint64, qint64))
           );
    connect(downloaderObject, SIGNAL(done(const QUrl&, const QByteArray&)),
            this,  SLOT(slotDone(const QUrl&, const QByteArray&))
           );
    connect(currencyNameLineEdit, &QLineEdit::textEdited,
            xmlParserObject, &XmlCurrencyParser::setCurrencyName
            );
    connect(currencyNameLineEdit, &QLineEdit::textChanged,
            xmlParserObject, &XmlCurrencyParser::setCurrencyName
            );
    connect(this, &DownloaderGui::loadedXml,
            xmlParserObject, &XmlCurrencyParser::getCurrecyValue
            );
    connect(xmlParserObject, &XmlCurrencyParser::currencyValueFound,
            currencyValueLabel, &QLabel::setText
            );
    connect(xmlParserObject, &XmlCurrencyParser::dateFound,
            dateLineEdit, &QLineEdit::setText
            );

    currencyValueLabel->setText(QLocale::system().decimalPoint());
}

// ----------------------------------------------------------------------
void DownloaderGui::slotGo()
{
    downloaderObject->download(QUrl(urlLineEdit->text()));
}

// ----------------------------------------------------------------------
void DownloaderGui::slotDownloadProgress(qint64 nReceived, qint64 nTotal)
{
    if (nTotal <= 0) {
        slotError();
        return;
    }
    downProgressBar->setValue(100 * nReceived / nTotal);
}

// ----------------------------------------------------------------------
void DownloaderGui::slotDone(const QUrl& url, const QByteArray& ba)
{
    //TODO check for valid XML doc
    //TODO rewrite woth lambda
    emit loadedXml(ba);
//    m_pxcp->getCurrecyValue(ba);
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
