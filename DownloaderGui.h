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
//  DownloaderGui.h
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

#pragma once

#include <QWidget>
#include <QUrl>
#include <QLocale>

class Downloader;
class QProgressBar;
class QLineEdit;
class QPushButton;
class XmlCurrencyParser;
class QLabel;

// ======================================================================
class DownloaderGui : public QWidget {
Q_OBJECT

private:
    Downloader*   downloaderObject;
    XmlCurrencyParser* xmlParserObject;

    QProgressBar* downProgressBar;
    QPushButton*  goButton;

    QLabel* currencyValueLabel; //valueToRUR
    QLabel* currencyLabel;
    QLabel* valueLable;
    QLabel* dateLable;
    QLineEdit*    currencyNameLineEdit; //currencyName
    QLineEdit* dateLineEdit;
    QLineEdit*    urlLineEdit;


public:
    DownloaderGui(QWidget* pwgt = 0);

signals:
    void loadedXml           (const QByteArray&             );
private slots:
    void slotGo              (                              );
    void slotError           (                              );
    void slotDownloadProgress(qint64, qint64                );
    void slotDone            (const QUrl&, const QByteArray&);
    void slotError           (const QString&                );
};

