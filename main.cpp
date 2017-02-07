#include <QApplication>
#include "Gui.h"

// ----------------------------------------------------------------------
int main(int argc, char** argv)
{
    QApplication  app(argc, argv);
    Gui downloader;

    //downloader.show();

    return app.exec();
}
