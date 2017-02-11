TEMPLATE     = app
QT          += widgets network xml sql
SOURCES      = main.cpp \
               Downloader.cpp \
               Gui.cpp \
               XmlParser.cpp \
    SqlModel.cpp
HEADERS      = Downloader.h \
               Gui.h \
               XmlParser.h \
    SqlModel.h

OTHER_FILES += \
    README.md \
    LICENSE \
    xml_examples/XML_daily.asp \
    xml_examples/XML_val.asp \
    queries.sql

TRANSLATIONS = qt-currency-quotes-CBRF_ru.ts qt-currency-quotes-CBRF_en.ts
QMAKE_POST_LINK += lupdate $$PWD/qt-currency-quotes-CBRF.pro

windows {
    TARGET = qt-currency-quotes-CBRF
    DESTDIR = ../currency-quotes-CBRF
    DESTDIR_TARGET = $$join(DESTDIR,,,TARGET)
#    QMAKE_POST_LINK += windeployqt $$OUT_PWD/currency-quotes-CBRF/currency-quotes-CBRF.exe
    QMAKE_POST_LINK += windeployqt $$join(DESTDIR_TARGET,".",,TARGET_EXT)
}
