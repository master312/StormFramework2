#pragma once
#include <QDockWidget>
#include <QMenu>

class SEDockWidget : public QDockWidget {
Q_OBJECT
public:
    explicit SEDockWidget(const QString &colorName, QMainWindow *parent = nullptr, Qt::WindowFlags flags = 0);
    void setCustomSizeHint(const QSize &size);
    QMenu *colorSwatchMenu() const { return menu; }
protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU
    void resizeEvent(QResizeEvent *e) override;
private slots:
    void changeClosable(bool on);
    void changeMovable(bool on);
    void changeFloatable(bool on);
    void changeFloating(bool on);
    void changeVerticalTitleBar(bool on);
    void updateContextMenu();
    void allowLeft(bool a);
    void allowRight(bool a);
    void allowTop(bool a);
    void allowBottom(bool a);
    void placeLeft(bool p);
    void placeRight(bool p);
    void placeTop(bool p);
    void placeBottom(bool p);
    void splitInto(QAction *action);
    void tabInto(QAction *action);
private:
    void allow(Qt::DockWidgetArea area, bool allow);
    void place(Qt::DockWidgetArea area, bool place);
    QAction *closableAction;
    QAction *movableAction;
    QAction *floatableAction;
    QAction *floatingAction;
    QAction *verticalTitleBarAction;
    QActionGroup *allowedAreasActions;
    QAction *allowLeftAction;
    QAction *allowRightAction;
    QAction *allowTopAction;
    QAction *allowBottomAction;
    QActionGroup *areaActions;
    QAction *leftAction;
    QAction *rightAction;
    QAction *topAction;
    QAction *bottomAction;
    QMenu *tabMenu;
    QMenu *splitHMenu;
    QMenu *splitVMenu;
    QMenu *menu;
    QMainWindow *mainWindow;

};
