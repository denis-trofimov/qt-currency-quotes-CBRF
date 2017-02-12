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
QMAKE_PRE_LINK += lrelease $$_PRO_FILE_

windows {
    TARGET = qt-currency-quotes-CBRF
    DESTDIR = qt-currency-quotes-CBRF
    QMAKE_POST_LINK += windeployqt $$OUT_PWD/$$DESTDIR/$$join(TARGET,,,".exe")
}

RESOURCES += \
    qt-currency-quotes-CBRF.qrc
