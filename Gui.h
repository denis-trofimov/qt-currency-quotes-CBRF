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


#pragma once

#include <QWidget>
#include <QUrl>
#include <QDate>
#include <QtWidgets>
#include "Downloader.h"
#include "XmlParser.h"
#include "SqlModel.h"

class Downloader;
class QProgressBar;
class QLineEdit;
class QPushButton;
class XmlParser;
class QLabel;
class QDate;

// ======================================================================
class Gui : public QWidget {
Q_OBJECT

private:
    Downloader*   downloaderObject;
    XmlParser* xmlParserObject;
    SqlModel* sqlModelObject;

    QProgressBar* downProgressBar;
    QPushButton*  goButton;


    QLabel* currencyCodeLabel;
    QLineEdit* currencyCodeLineEdit;
    QLabel* dateLabel;
    QLineEdit* dateLineEdit;

    QLabel* valueLabel;
    QLabel* valueResultLabel;
    QLabel* nominalLabel;
    QLabel* nominalResultLabel;
    QLabel* nameResultLabel;
    QLabel* nameLabel;

    QLineEdit* urlLineEdit;

    QString currencyCode;
    QString value;
    QDate date;
    QString nominal;
    QString name;

public:
    Gui(QWidget* pwgt = 0);

signals:
    void loadedXml           (const QByteArray&             );

private slots:
    void slotGo              (                              );
    void slotDownloadProgress(qint64, qint64                );
    void slotDone            (const QUrl&, const QByteArray&);
    void slotError           (const QString& errorMessage =
            "An error while download is occured");
//    void slotError           (               );
    void slotParseSucces(const QString& valueParsed,
                         const QString& nominalParsed,
                         const QString& nameParsed);
};

