#include "SEToolbar.h"
#include "MainWindow.h"

SEToolbar::SEToolbar(QWidget* parent, const QString& name) : QToolBar(parent) {
    setIconSize(QSize(22, 22));

    setObjectName(name);
    setWindowTitle(name);
    _ViewMenuToggle = nullptr;

    addToViewMenu();

    connect(this, &QToolBar::visibilityChanged, this, &SEToolbar::cbViewBarToggle);
}

SEToolbar::~SEToolbar() {
}

QFont& SEToolbar::getToolFont() {
    static QFont font;
    font.setPointSize(9);
    font.setItalic(true);
    return font;
}

void SEToolbar::cbViewBarToggle(bool isChecked) {
    setVisible(isChecked);
}

void SEToolbar::cbVisibilityChanged(bool isVisible) {
    _ViewMenuToggle->setChecked(isVisible);
}

void SEToolbar::addToViewMenu() {
    QMenu* viewBar = MainWindow::get()->getMenu("ViewToolbars");
    if (!viewBar) {
        return;
    }
    _ViewMenuToggle = viewBar->addAction(objectName());
    _ViewMenuToggle->setCheckable(true);
    _ViewMenuToggle->setChecked(true);
    connect(_ViewMenuToggle, &QAction::toggled, this, &SEToolbar::cbViewBarToggle);
}