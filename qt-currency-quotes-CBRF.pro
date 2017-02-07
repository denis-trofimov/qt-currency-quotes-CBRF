TEMPLATE     = app
QT          += widgets network xml sql
SOURCES      = main.cpp \
               Downloader.cpp \
               Gui.cpp \
               XmlParser.cpp \
    sqlmodel.cpp
HEADERS      = Downloader.h \
               Gui.h \
               XmlParser.h \
    sqlmodel.h
windows:TARGET = ../qt-currency-quotes-CBRF

OTHER_FILES += \
    README.md \
    LICENSE \
    xml_examples/XML_daily.asp \
    xml_examples/XML_val.asp \

