#include <StormEngine.h>
INITIALIZE_EASYLOGGINGPP

#include <QResource>
#include <QApplication>
#include <QPainterPath>
#include <QPainter>
#include "MainWindow.h"

int main(int argc, char **argv)  {
    QApplication app(argc, argv);
    QResource::registerResource("editor_data/resources.qrc");
    MainWindow mainWin;
    mainWin.resize(1280, 768);
    mainWin.show();
    return app.exec();
}