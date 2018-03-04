#pragma once
#include <QMainWindow>
#include <QMenu>
#include <map>

class SEDockObjectHierarchy;
class SEDockObjectComponents;


class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();

    static MainWindow* get();

    QMenu* getMenu(const std::string& name);

    static SEDockObjectHierarchy* getHierarchyDock();

    /* Returns name of editor executable */
    QString getExecutableName();

public slots:
    void cbMenuOpenScene();
    void cbMenuSaveSceneAs();
    void cbMenuSaveLayout();
    void cbMenuLoadLayout();

    /* Creates all docked widgets.
     * Should be called just after engine is done initializing */
    void setupDockedWidgets();

private:
    std::map<std::string, QMenu*> _Menus;

    SEDockObjectHierarchy* _ObjectHierarchyDock;
    SEDockObjectComponents* _ObjectComoponentsDock;


    void setupMenuBar();
    void setupToolbars();
    void setWindowDockingOptions();
};
