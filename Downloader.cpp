// ======================================================================
//  Downloader.cpp
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

#include <QtNetwork>
#include "Downloader.h"

// ----------------------------------------------------------------------
Downloader::Downloader(QObject* pobj/*=0*/) : QObject(pobj)
{ 
    m_pnam = new QNetworkAccessManager(this);
    connect(m_pnam, SIGNAL(finished(QNetworkReply*)), 
            this,   SLOT(slotFinished(QNetworkReply*))
           );
}

// ----------------------------------------------------------------------
void Downloader::download(const QUrl& url)
{
    QNetworkRequest request(url);
    QNetworkReply*  pnr = m_pnam->get(request);
    connect(pnr, SIGNAL(downloadProgress(qint64, qint64)), 
            this, SIGNAL(downloadProgress(qint64, qint64))
           );
}

// ----------------------------------------------------------------------
void Downloader::slotFinished(QNetworkReply* pnr)
{
    if (pnr->error() != QNetworkReply::NoError) {
        emit error();
    }
    else {
        emit done(pnr->url(), pnr->readAll());
    }
    pnr->deleteLater();
}
