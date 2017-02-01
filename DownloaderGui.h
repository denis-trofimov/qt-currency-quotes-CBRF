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
    Downloader*   m_pdl;
    QProgressBar* m_ppb;
    QLineEdit*    m_ptxt;
    QPushButton*  m_pcmd;
    XmlCurrencyParser* m_pxcp;
    QLabel* m_pcv; //valueToRUR
    QLineEdit*    m_pcn; //currencyName

    void showPic(const QString&);

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

