#include <QApplication>
#include "DownloaderGui.h"

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication  app(argc, argv);
    Gui downloader;

    downloader.show();

    return app.exec();
}
