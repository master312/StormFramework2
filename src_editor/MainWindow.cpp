#include "MainWindow.h"
#include "toolbars/SEFileToolbar.h"
#include "dock_widgets/SEDockWidget.h"

#include <QTextEdit>

MainWindow::MainWindow() : QMainWindow(nullptr) {
    //ToolBar *tb = new ToolBar(QString::fromLatin1("Tool Bar %1").arg(0 + 1), this);
    //addToolBar(tb);

    QTextEdit *center = new QTextEdit(this);
    center->setReadOnly(true);
    center->setMinimumSize(400, 205);
    setCentralWidget(center);

    SEDockWidget* w = new SEDockWidget("Asd", this);
    addDockWidget(Qt::RightDockWidgetArea, w);

    w = new SEDockWidget("Asd2", this);
    addDockWidget(Qt::LeftDockWidgetArea, w);
    w = new SEDockWidget("Asd42", this);
    addDockWidget(Qt::LeftDockWidgetArea, w);
    w = new SEDockWidget("Asd562", this);
    addDockWidget(Qt::LeftDockWidgetArea, w);
    w = new SEDockWidget("As7d2", this);
    addDockWidget(Qt::LeftDockWidgetArea, w);

    setWindowDockingOptions();

    initToolbars();
}

MainWindow::~MainWindow() {

}

void MainWindow::setWindowDockingOptions() {
    QMainWindow::setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
    QMainWindow::setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);

    DockOptions opts;
    opts |= AnimatedDocks;
    opts |= VerticalTabs;
    QMainWindow::setDockOptions(opts);
}

void MainWindow::initToolbars() {
    SEFileToolbar* fileToolbar = new SEFileToolbar(this);
    addToolBar(fileToolbar);
}