#pragma once
#include <QMainWindow>
#include <QMenu>
#include <map>
#include "docks/object_hierarchy/SEDockObjectHierarchy.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();

    static MainWindow* get();

    QMenu* getMenu(const std::string& name);

    SEDockObjectHierarchy* getHierarchyWidget();

public slots:
    void cbMenuOpenScene();
    void cbMenuSaveScene();
    void cbMenuSaveLayout();
    void cbMenuLoadLayout();


private:
    std::map<std::string, QMenu*> _Menus;
    SEDockObjectHierarchy* _ObjectHierarchyWidget;


    void setupMenuBar();
    void setupToolbars();
    void setupDockedWidgets();
    void setWindowDockingOptions();
};
