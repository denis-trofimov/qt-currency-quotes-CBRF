TEMPLATE     = app
QT          += widgets network xml
SOURCES      = main.cpp \
               Downloader.cpp \
               DownloaderGui.cpp \
               XmlParser.cpp
HEADERS      = Downloader.h \
               Gui.h \
               XmlParser.h \
    Gui.h
windows:TARGET = ../qt-currency-quotes-CBRF

OTHER_FILES += \
    README.md \
    LICENSE \
    xml_examples/XML_daily.asp \
    xml_examples/XML_val.asp \

