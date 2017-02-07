#pragma once

#include <QWidget>
#include <QUrl>
#include <QDate>

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
//    QString valueToRUR;
    QDate date;
//    QString nominal;
//    QString currencyName;

public:
    Gui(QWidget* pwgt = 0);

signals:
    void loadedXml           (const QByteArray&             );

private slots:
    void slotGo              (                              );
    void slotDownloadProgress(qint64, qint64                );
    void slotDone            (const QUrl&, const QByteArray&);
    void slotError           (const QString&                );
    void slotError           (               );
    void slotParseSucces(const QString& valueParsed,
                         const QString& nominalParsed,
                         const QString& nameParsed);
};

