TEMPLATE     = app
QT          += widgets network xml
SOURCES	     = main.cpp \
               Downloader.cpp \
               DownloaderGui.cpp \
               XmlCurrencyParser.cpp
HEADERS      = Downloader.h \
               DownloaderGui.h \ 
               XmlCurrencyParser.h
windows:TARGET = ../Downloader

