#include <QtWidgets>
#include "Downloader.h"
#include "Gui.h"
#include "XmlParser.h"
#include ""

// ----------------------------------------------------------------------
Gui::Gui(QWidget* pwgt /*=0*/) : QWidget(pwgt)
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

//    xmlParserObject = new XmlParser(this);
    xmlParserObject = new XmlParser(this, currencyCode);
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
}

// ----------------------------------------------------------------------
void Gui::slotGo()
{
    QDate inputDate = QDate::fromString(dateLineEdit->text(),"dd.MM.yyyy");
    if(!inputDate.isValid()
            || inputDate > (QDate().currentDate()).addDays(1)
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
void Gui::slotDownloadProgress(qint64 nReceived, qint64 nTotal)
{
    if (nTotal <= 0) {
        slotError(tr("An error while download is occured"));
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
void Gui::slotError()
{
    QMessageBox::critical(0,
                          tr("Error"),
                          tr("An error while download is occured")
                         );
}

// ----------------------------------------------------------------------
void Gui::slotError(const QString& errorMessage =
        "An error while download is occured")
{
    QMessageBox::critical(0,
                          tr("Error"),
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
}
