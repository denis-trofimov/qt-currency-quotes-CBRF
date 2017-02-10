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
windows:TARGET = ../currency-quotes-CBRF/currency-quotes-CBRF

OTHER_FILES += \
    README.md \
    LICENSE \
    xml_examples/XML_daily.asp \
    xml_examples/XML_val.asp \
    queries.sql

windows:QMAKE_POST_LINK += windeployqt $$OUT_PWD/currency-quotes-CBRF/currency-quotes-CBRF.exe
