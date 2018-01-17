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
    setContentsMargins(1, 10, 1, 1);

    setFeatures(features() & ~DockWidgetFloatable);

    addToViewMenu();

    _RootWidget = new QWidget(this);
    setWidget(_RootWidget);

    connect(this, &QDockWidget::visibilityChanged, this, &SEDockWidget::cbVisibilityChanged);
}

void SEDockWidget::cbViewBarToggle(bool isChecked) {
    setVisible(isChecked);
}

void SEDockWidget::cbVisibilityChanged(bool isVisible) {
    _ViewMenuToggle->setChecked(isVisible);
}

QLayout* SEDockWidget::genericBoxLayout(SEDockWidget* parent) {
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(1);
    layout->setMargin(0);
    layout->setAlignment(Qt::AlignmentFlag::AlignTop);
    return static_cast<QLayout*>(layout);
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