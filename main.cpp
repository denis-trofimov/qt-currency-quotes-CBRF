#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "Gui.h"

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication  app(argc, argv);

    QTranslator translator;

    translator.load(QLocale::system(),
                    "qt-currency-quotes-CBRF", "_", ":/", ".qm");

//  translator.load("qt-currency-quotes-CBRF_en.qm", ":/");

    app.installTranslator(&translator);

    Gui downloader;

    downloader.show();

    return app.exec();
}
