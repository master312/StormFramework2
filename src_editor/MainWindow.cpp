#include "MainWindow.h"
#include "toolbars/SEFileToolbar.h"
#include "toolbars/SFEditToolbar.h"
#include "dock_widgets/SEDockWidget.h"
#include <QFileDialog>
#include <QMenuBar>
#include <QTextEdit>
#include <QMessageBox>

/* Main windows 'fake singleton' holder for easy access */
static MainWindow* mainWindow = nullptr;

MainWindow::MainWindow() : QMainWindow(nullptr) {
    mainWindow = this;

    /* Create central widget. */
    /* TODO: Should be OpenGL renderer widget */
    QTextEdit* center = new QTextEdit(this);
    center->setReadOnly(true);
    center->setMinimumSize(400, 205);
    setCentralWidget(center);

    setWindowDockingOptions();

    setupMenuBar();
    setupToolbars();
    setupDockedWidgets();
}

MainWindow::~MainWindow() {
}

MainWindow* MainWindow::get() {
    return mainWindow;
}

QMenu* MainWindow::getMenu(const std::string& name) {
    auto iter = _Menus.find(name);
    if (iter != _Menus.end()) {
        return iter->second;
    }
    return nullptr;
}

void MainWindow::setupMenuBar() {
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(tr("Open scene"), this, &MainWindow::cbMenuOpenScene);
    fileMenu->addAction(tr("Save scene"), this, &MainWindow::cbMenuSaveScene);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("&Quit"), this, &QWidget::close);
    _Menus["File"] = fileMenu;

    QMenu* viewMenu = menuBar()->addMenu(tr("&View"));
    _Menus["ViewDocks"] = viewMenu->addMenu("Docks");
    _Menus["ViewToolbars"] = viewMenu->addMenu("Toolbars");
    viewMenu->addSeparator();
    viewMenu->addAction(tr("Save layout"), this, &MainWindow::cbMenuSaveLayout);
    viewMenu->addAction(tr("Load layout"), this, &MainWindow::cbMenuLoadLayout);
    _Menus["View"] = viewMenu;
}

void MainWindow::setupDockedWidgets() {
    SEDockWidget* dockWidget = new SEDockWidget(this, "Object hierarchy");
    dockWidget->setMinimumWidth(130);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    dockWidget = new SEDockWidget(this, "Object Components");
    dockWidget->setMinimumWidth(140);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    dockWidget = new SEDockWidget(this, "Files Manager");
    addDockWidget(Qt::BottomDockWidgetArea, dockWidget);

    dockWidget = new SEDockWidget(this, "Some random tab");
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);
}

void MainWindow::setupToolbars() {
    SEFileToolbar* fileToolbar = new SEFileToolbar(this);
    addToolBar(fileToolbar);

    SFEditToolbar* toolsToolbar = new SFEditToolbar(this);
    addToolBar(toolsToolbar);
}

void MainWindow::setWindowDockingOptions() {
    QMainWindow::setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
    QMainWindow::setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);

    DockOptions opts;
    opts |= AnimatedDocks;
    //opts |= VerticalTabs;
    QMainWindow::setDockOptions(opts);
}

void MainWindow::cbMenuOpenScene() {

}

void MainWindow::cbMenuSaveScene() {

}

void MainWindow::cbMenuSaveLayout() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save layout"));
    if (fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QString msg = tr("Failed to open %1\n%2")
                .arg(QDir::toNativeSeparators(fileName), file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
    QByteArray geo_data = saveGeometry();
    QByteArray layout_data = saveState();
    bool ok = file.putChar((uchar)geo_data.size());
    if (ok) {
        ok = file.write(geo_data) == geo_data.size();
    }
    if (ok) {
        ok = file.write(layout_data) == layout_data.size();
    }
    if (!ok) {
        QString msg = tr("Error writing to %1\n%2")
                .arg(QDir::toNativeSeparators(fileName), file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
}

void MainWindow::cbMenuLoadLayout() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load layout"));
    if (fileName.isEmpty()) {
        return;
    }
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QString msg = tr("Failed to open %1\n%2")
                .arg(QDir::toNativeSeparators(fileName), file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
    uchar geo_size;
    QByteArray geo_data;
    QByteArray layout_data;
    bool ok = file.getChar((char*)&geo_size);
    if (ok) {
        geo_data = file.read(geo_size);
        ok = geo_data.size() == geo_size;
    }
    if (ok) {
        layout_data = file.readAll();
        ok = layout_data.size() > 0;
    }
    if (ok) {
        ok = restoreGeometry(geo_data);
    }
    if (ok) {
        ok = restoreState(layout_data);
    }
    if (!ok) {
        QString msg = tr("Error reading %1").arg(QDir::toNativeSeparators(fileName));
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
}