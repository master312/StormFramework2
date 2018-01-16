#include "SEDockWidget.h"
#include <QtEvents>
#include <QMainWindow>
#include <QPainter>
#include <QGridLayout>
#include <QPushButton>
#include "MainWindow.h"

SEDockWidget::SEDockWidget(QMainWindow* parent, const QString& name) : QDockWidget(parent) {
    setObjectName("DOCK" + name);
    setWindowTitle(name);
    _ViewMenuToggle = nullptr;

    setMinimumSize(100, 100);

    setFeatures(features() & ~DockWidgetFloatable);

    addToViewMenu();

    connect(this, &QDockWidget::visibilityChanged, this, &SEDockWidget::cbVisibilityChanged);
}

void SEDockWidget::cbViewBarToggle(bool isChecked) {
    setVisible(isChecked);
}

void SEDockWidget::cbVisibilityChanged(bool isVisible) {
    _ViewMenuToggle->setChecked(isVisible);
}

void SEDockWidget::addToViewMenu() {
    QMenu* viewBar = MainWindow::get()->getMenu("ViewDocks");
    if (!viewBar) {
        return;
    }
    _ViewMenuToggle = viewBar->addAction(windowTitle());
    _ViewMenuToggle->setCheckable(true);
    _ViewMenuToggle->setChecked(true);
    connect(_ViewMenuToggle, &QAction::toggled, this, &SEDockWidget::cbViewBarToggle);
}

#include "SEDockWidget.moc"