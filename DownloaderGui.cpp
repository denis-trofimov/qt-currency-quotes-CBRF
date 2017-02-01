// ======================================================================
//  DownloaderGui.cpp
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
    m_pdl  = new Downloader(this);
    m_ppb  = new QProgressBar;
    m_ptxt = new QLineEdit;
    m_pcmd = new QPushButton(tr("&Go"));
    m_pcv = new QLabel();
    m_pxcp = new XmlCurrencyParser(this);
    m_pcn = new QLineEdit;

    connect(m_pcmd, SIGNAL(clicked()), SLOT(slotGo()));
    connect(m_pdl, SIGNAL(downloadProgress(qint64, qint64)),  
            this,  SLOT(slotDownloadProgress(qint64, qint64)) 
           );
    connect(m_pdl, SIGNAL(done(const QUrl&, const QByteArray&)),  
            this,  SLOT(slotDone(const QUrl&, const QByteArray&)) 
           );
    connect(m_pcn, &QLineEdit::textEdited,
            m_pxcp, &XmlCurrencyParser::setCurrencyName
            );
    connect(m_pcn, &QLineEdit::textChanged,
            m_pxcp, &XmlCurrencyParser::setCurrencyName
            );
    connect(this, &DownloaderGui::loadedXml,
            m_pxcp, &XmlCurrencyParser::getCurrecyValue
            );
    connect(m_pxcp, SIGNAL(valuefound(double)),
            m_pcv, SLOT(setNum(double))
            );

    QString strDownloadLink =
            "http://www.cbr.ru/scripts/XML_daily.asp";
    m_ptxt->setText(strDownloadLink);
    m_pcn->setText(tr("USD"));

    QGridLayout* pLayout = new QGridLayout;

    pLayout->addWidget(m_ptxt, 0, 0);
    pLayout->addWidget(m_pcmd, 0, 1);
    pLayout->addWidget(m_ppb, 1, 0, 1, 1);
    pLayout->addWidget(m_pcn, 2, 0);
    pLayout->addWidget(m_pcv, 2, 1);
    setLayout(pLayout);
}

// ----------------------------------------------------------------------
void DownloaderGui::slotGo()
{
    m_pdl->download(QUrl(m_ptxt->text()));
}

// ----------------------------------------------------------------------
void DownloaderGui::slotDownloadProgress(qint64 nReceived, qint64 nTotal)
{
    if (nTotal <= 0) {
        slotError();
        return;
    }
    m_ppb->setValue(100 * nReceived / nTotal);
}

// ----------------------------------------------------------------------
void DownloaderGui::slotDone(const QUrl& url, const QByteArray& ba)
{
    TODO://TODO check for XML doc
    emit loadedXml(ba);
//    m_pxcp->getCurrecyValue(ba);


    //    QString strFileName = url.path().section('/', -1);
//    QFile   file(strFileName);
//    if (file.open(QIODevice::WriteOnly)) {
//        file.write(ba);
//        file.close();

//        if (strFileName.endsWith(".jpg")
//            || strFileName.endsWith(".png")
//           ) {
//            showPic(strFileName);
//        }
//    }
}

// ----------------------------------------------------------------------
void DownloaderGui::showPic(const QString& strFileName)
{
    QPixmap pix(strFileName);
    pix = pix.scaled(pix.size() / 3, 
                     Qt::IgnoreAspectRatio, 
                     Qt::SmoothTransformation
                    );
    QLabel* plbl = new QLabel;
    plbl->setPixmap(pix);
    plbl->setFixedSize(pix.size());
    plbl->show();
}

// ----------------------------------------------------------------------
void DownloaderGui::slotError()
{
    QMessageBox::critical(0,
                          tr("Error"),
                          tr("An error while download is occured")
                         );
}

void DownloaderGui::slotError(const QString& errorMessage = "An error while download is occured")
{
    QMessageBox::critical(0,
                          tr("Error"),
                          errorMessage
                         );
}
