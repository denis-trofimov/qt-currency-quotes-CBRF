#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "Gui.h"

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication  app(argc, argv);

    QTranslator translator;



//    QStringList localesList = QLocale::system().uiLanguages();

    bool check1 = translator.load(QLocale::system(),
                                 "qt-currency-quotes-CBRF", "_", ":/", ".qm");
//    bool check2, check3;
//    QString mylocale = QLocale::languageToString(QLocale::system().language());
//    if(QLocale::system().language() == QLocale::Russian)
//    {
//        //://qt-currency-quotes-CBRF_ru.qm
//        check2 = translator.load("qt-currency-quotes-CBRF_ru.qm", ":/");
//    }
//    else
//        check3 = translator.load("qt-currency-quotes-CBRF_en.qm", ":/");

    app.installTranslator(&translator);

    Gui downloader;

    downloader.show();

    return app.exec();
}
