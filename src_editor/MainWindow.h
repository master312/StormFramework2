#pragma once
#include <QMainWindow>
#include <QMenu>
#include <map>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();

    QMenu* getMenu(const std::string& name);

public slots:
    void cbMenuOpenScene();
    void cbMenuSaveScene();
    void cbMenuSaveLayout();
    void cbMenuLoadLayout();


private:
    std::map<std::string, QMenu*> _Menus;

    void setupMenuBar();
    void setupToolbars();
    void setupDockedWidgets();
    void setWindowDockingOptions();
};
