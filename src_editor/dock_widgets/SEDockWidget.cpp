#include "SEDockWidget.h"
#include <QAction>
#include <QtEvents>
#include <QFrame>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QLabel>
#include <QPainterPath>
#include <QPushButton>
#include <QHBoxLayout>
#include <QBitmap>
#include <QtDebug>
#include <QMenu>
#include <QMenuBar>
#include "../MainWindow.h"
SEDockWidget::SEDockWidget(const QString &colorName, QMainWindow *parent, Qt::WindowFlags flags)
        : QDockWidget(parent, flags), mainWindow(parent)
{
    setObjectName(colorName + QLatin1String(" Dock Widget"));
    setWindowTitle(objectName() + QLatin1String(" [*]"));
    closableAction = new QAction(tr("Closable"), this);
    closableAction->setCheckable(true);
    connect(closableAction, &QAction::triggered, this, &SEDockWidget::changeClosable);
    movableAction = new QAction(tr("Movable"), this);
    movableAction->setCheckable(true);
    connect(movableAction, &QAction::triggered, this, &SEDockWidget::changeMovable);
    floatableAction = new QAction(tr("Floatable"), this);
    floatableAction->setCheckable(true);
    connect(floatableAction, &QAction::triggered, this, &SEDockWidget::changeFloatable);
    verticalTitleBarAction = new QAction(tr("Vertical title bar"), this);
    verticalTitleBarAction->setCheckable(true);
    connect(verticalTitleBarAction, &QAction::triggered,
            this, &SEDockWidget::changeVerticalTitleBar);
    floatingAction = new QAction(tr("Floating"), this);
    floatingAction->setCheckable(true);
    connect(floatingAction, &QAction::triggered, this, &SEDockWidget::changeFloating);
    allowedAreasActions = new QActionGroup(this);
    allowedAreasActions->setExclusive(false);
    allowLeftAction = new QAction(tr("Allow on Left"), this);
    allowLeftAction->setCheckable(true);
    connect(allowLeftAction, &QAction::triggered, this, &SEDockWidget::allowLeft);
    allowRightAction = new QAction(tr("Allow on Right"), this);
    allowRightAction->setCheckable(true);
    connect(allowRightAction, &QAction::triggered, this, &SEDockWidget::allowRight);
    allowTopAction = new QAction(tr("Allow on Top"), this);
    allowTopAction->setCheckable(true);
    connect(allowTopAction, &QAction::triggered, this, &SEDockWidget::allowTop);
    allowBottomAction = new QAction(tr("Allow on Bottom"), this);
    allowBottomAction->setCheckable(true);
    connect(allowBottomAction, &QAction::triggered, this, &SEDockWidget::allowBottom);
    allowedAreasActions->addAction(allowLeftAction);
    allowedAreasActions->addAction(allowRightAction);
    allowedAreasActions->addAction(allowTopAction);
    allowedAreasActions->addAction(allowBottomAction);
    areaActions = new QActionGroup(this);
    areaActions->setExclusive(true);
    leftAction = new QAction(tr("Place on Left") , this);
    leftAction->setCheckable(true);
    connect(leftAction, &QAction::triggered, this, &SEDockWidget::placeLeft);
    rightAction = new QAction(tr("Place on Right") , this);
    rightAction->setCheckable(true);
    connect(rightAction, &QAction::triggered, this, &SEDockWidget::placeRight);
    topAction = new QAction(tr("Place on Top") , this);
    topAction->setCheckable(true);
    connect(topAction, &QAction::triggered, this, &SEDockWidget::placeTop);
    bottomAction = new QAction(tr("Place on Bottom") , this);
    bottomAction->setCheckable(true);
    connect(bottomAction, &QAction::triggered, this, &SEDockWidget::placeBottom);
    areaActions->addAction(leftAction);
    areaActions->addAction(rightAction);
    areaActions->addAction(topAction);
    areaActions->addAction(bottomAction);
    connect(movableAction, &QAction::triggered, areaActions, &QActionGroup::setEnabled);
    connect(movableAction, &QAction::triggered, allowedAreasActions, &QActionGroup::setEnabled);
    connect(floatableAction, &QAction::triggered, floatingAction, &QAction::setEnabled);
    connect(floatingAction, &QAction::triggered, floatableAction, &QAction::setDisabled);
    connect(movableAction, &QAction::triggered, floatableAction, &QAction::setEnabled);
    tabMenu = new QMenu(this);
    tabMenu->setTitle(tr("Tab into"));
    connect(tabMenu, &QMenu::triggered, this, &SEDockWidget::tabInto);
    splitHMenu = new QMenu(this);
    splitHMenu->setTitle(tr("Split horizontally into"));
    connect(splitHMenu, &QMenu::triggered, this, &SEDockWidget::splitInto);
    splitVMenu = new QMenu(this);
    splitVMenu->setTitle(tr("Split vertically into"));
    connect(splitVMenu, &QMenu::triggered, this, &SEDockWidget::splitInto);
    QAction *windowModifiedAction = new QAction(tr("Modified"), this);
    windowModifiedAction->setCheckable(true);
    windowModifiedAction->setChecked(false);
    connect(windowModifiedAction, &QAction::toggled, this, &QWidget::setWindowModified);
    menu = new QMenu(colorName, this);
    menu->addAction(toggleViewAction());
    menu->addAction(tr("Raise"), this, &QWidget::raise);
    //menu->addAction(tr("Change Size Hints..."), swatch, &ColorDock::changeSizeHints);
    menu->addSeparator();
    menu->addAction(closableAction);
    menu->addAction(movableAction);
    menu->addAction(floatableAction);
    menu->addAction(floatingAction);
    menu->addAction(verticalTitleBarAction);
    menu->addSeparator();
    menu->addActions(allowedAreasActions->actions());
    menu->addSeparator();
    menu->addActions(areaActions->actions());
    menu->addSeparator();
    menu->addMenu(splitHMenu);
    menu->addMenu(splitVMenu);
    menu->addMenu(tabMenu);
    menu->addSeparator();
    menu->addAction(windowModifiedAction);
    connect(menu, &QMenu::aboutToShow, this, &SEDockWidget::updateContextMenu);
    if (colorName == QLatin1String("Black")) {
        leftAction->setShortcut(Qt::CTRL | Qt::Key_W);
        rightAction->setShortcut(Qt::CTRL | Qt::Key_E);
        toggleViewAction()->setShortcut(Qt::CTRL | Qt::Key_R);
    }

    QMenu* viewBar = dynamic_cast<MainWindow*>(parent)->getMenu("View");
    viewMenuCheckAction = viewBar->addAction(objectName());
    viewMenuCheckAction->setCheckable(true);
    viewMenuCheckAction->setChecked(true);
    connect(viewMenuCheckAction, &QAction::toggled, this, &SEDockWidget::cbViewBarToggle);
    setFeatures(features() & ~DockWidgetFloatable);
}

void SEDockWidget::cbViewBarToggle(bool isChecked) {
    setVisible(isChecked);
}

void SEDockWidget::updateContextMenu()
{
    const Qt::DockWidgetArea area = mainWindow->dockWidgetArea(this);
    const Qt::DockWidgetAreas areas = allowedAreas();
    closableAction->setChecked(features() & QDockWidget::DockWidgetClosable);
    if (windowType() == Qt::Drawer) {
        floatableAction->setEnabled(false);
        floatingAction->setEnabled(false);
        movableAction->setEnabled(false);
        verticalTitleBarAction->setChecked(false);
    } else {
        floatableAction->setChecked(features() & QDockWidget::DockWidgetFloatable);
        floatingAction->setChecked(isWindow());
        // done after floating, to get 'floatable' correctly initialized
        movableAction->setChecked(features() & QDockWidget::DockWidgetMovable);
        verticalTitleBarAction
                ->setChecked(features() & QDockWidget::DockWidgetVerticalTitleBar);
    }
    allowLeftAction->setChecked(isAreaAllowed(Qt::LeftDockWidgetArea));
    allowRightAction->setChecked(isAreaAllowed(Qt::RightDockWidgetArea));
    allowTopAction->setChecked(isAreaAllowed(Qt::TopDockWidgetArea));
    allowBottomAction->setChecked(isAreaAllowed(Qt::BottomDockWidgetArea));
    if (allowedAreasActions->isEnabled()) {
        allowLeftAction->setEnabled(area != Qt::LeftDockWidgetArea);
        allowRightAction->setEnabled(area != Qt::RightDockWidgetArea);
        allowTopAction->setEnabled(area != Qt::TopDockWidgetArea);
        allowBottomAction->setEnabled(area != Qt::BottomDockWidgetArea);
    }
    {
        const QSignalBlocker blocker(leftAction);
        leftAction->setChecked(area == Qt::LeftDockWidgetArea);
    }
    {
        const QSignalBlocker blocker(rightAction);
        rightAction->setChecked(area == Qt::RightDockWidgetArea);
    }
    {
        const QSignalBlocker blocker(topAction);
        topAction->setChecked(area == Qt::TopDockWidgetArea);
    }
    {
        const QSignalBlocker blocker(bottomAction);
        bottomAction->setChecked(area == Qt::BottomDockWidgetArea);
    }
    if (areaActions->isEnabled()) {
        leftAction->setEnabled(areas & Qt::LeftDockWidgetArea);
        rightAction->setEnabled(areas & Qt::RightDockWidgetArea);
        topAction->setEnabled(areas & Qt::TopDockWidgetArea);
        bottomAction->setEnabled(areas & Qt::BottomDockWidgetArea);
    }
    tabMenu->clear();
    splitHMenu->clear();
    splitVMenu->clear();
    QList<SEDockWidget*> dock_list = mainWindow->findChildren<SEDockWidget*>();
    foreach (SEDockWidget *dock, dock_list) {
        tabMenu->addAction(dock->objectName());
        splitHMenu->addAction(dock->objectName());
        splitVMenu->addAction(dock->objectName());
    }
}
static SEDockWidget *findByName(const QMainWindow *mainWindow, const QString &name)
{
    foreach (SEDockWidget *dock, mainWindow->findChildren<SEDockWidget*>()) {
        if (name == dock->objectName())
            return dock;
    }
    return nullptr;
}
void SEDockWidget::splitInto(QAction *action)
{
    SEDockWidget *target = findByName(mainWindow, action->text());
    if (!target)
        return;
    const Qt::Orientation o = action->parent() == splitHMenu
                              ? Qt::Horizontal : Qt::Vertical;
    mainWindow->splitDockWidget(target, this, o);
}
void SEDockWidget::tabInto(QAction *action)
{
    if (SEDockWidget *target = findByName(mainWindow, action->text()))
        mainWindow->tabifyDockWidget(target, this);
}
#ifndef QT_NO_CONTEXTMENU
void SEDockWidget::contextMenuEvent(QContextMenuEvent *event)
{
    event->accept();
    menu->exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU
void SEDockWidget::resizeEvent(QResizeEvent *e)
{
//    if (BlueTitleBar *btb = qobject_cast<BlueTitleBar*>(titleBarWidget()))
//        btb->updateMask();
    QDockWidget::resizeEvent(e);
}

void SEDockWidget::closeEvent(QCloseEvent *event) {
    QDockWidget::closeEvent(event);
    viewMenuCheckAction->setChecked(false);
}

void SEDockWidget::allow(Qt::DockWidgetArea area, bool a)
{
    Qt::DockWidgetAreas areas = allowedAreas();
    areas = a ? areas | area : areas & ~area;
    setAllowedAreas(areas);
    if (areaActions->isEnabled()) {
        leftAction->setEnabled(areas & Qt::LeftDockWidgetArea);
        rightAction->setEnabled(areas & Qt::RightDockWidgetArea);
        topAction->setEnabled(areas & Qt::TopDockWidgetArea);
        bottomAction->setEnabled(areas & Qt::BottomDockWidgetArea);
    }
}
void SEDockWidget::place(Qt::DockWidgetArea area, bool p)
{
    if (!p)
        return;
    mainWindow->addDockWidget(area, this);
    if (allowedAreasActions->isEnabled()) {
        allowLeftAction->setEnabled(area != Qt::LeftDockWidgetArea);
        allowRightAction->setEnabled(area != Qt::RightDockWidgetArea);
        allowTopAction->setEnabled(area != Qt::TopDockWidgetArea);
        allowBottomAction->setEnabled(area != Qt::BottomDockWidgetArea);
    }
}
void SEDockWidget::setCustomSizeHint(const QSize &size)
{
//    if (ColorDock *dock = qobject_cast<ColorDock*>(widget()))
//        dock->setCustomSizeHint(size);
}
void SEDockWidget::changeClosable(bool on)
{ setFeatures(on ? features() | DockWidgetClosable : features() & ~DockWidgetClosable); }
void SEDockWidget::changeMovable(bool on)
{ setFeatures(on ? features() | DockWidgetMovable : features() & ~DockWidgetMovable); }
void SEDockWidget::changeFloatable(bool on)
{ setFeatures(on ? features() | DockWidgetFloatable : features() & ~DockWidgetFloatable); }
void SEDockWidget::changeFloating(bool floating)
{ setFloating(floating); }
void SEDockWidget::allowLeft(bool a)
{ allow(Qt::LeftDockWidgetArea, a); }
void SEDockWidget::allowRight(bool a)
{ allow(Qt::RightDockWidgetArea, a); }
void SEDockWidget::allowTop(bool a)
{ allow(Qt::TopDockWidgetArea, a); }
void SEDockWidget::allowBottom(bool a)
{ allow(Qt::BottomDockWidgetArea, a); }
void SEDockWidget::placeLeft(bool p)
{ place(Qt::LeftDockWidgetArea, p); }
void SEDockWidget::placeRight(bool p)
{ place(Qt::RightDockWidgetArea, p); }
void SEDockWidget::placeTop(bool p)
{ place(Qt::TopDockWidgetArea, p); }
void SEDockWidget::placeBottom(bool p)
{ place(Qt::BottomDockWidgetArea, p); }
void SEDockWidget::changeVerticalTitleBar(bool on)
{
    setFeatures(on ? features() | DockWidgetVerticalTitleBar
                   : features() & ~DockWidgetVerticalTitleBar);
}

#include "SEDockWidget.moc"