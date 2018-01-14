#pragma once
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    virtual ~MainWindow();

private:
    void setWindowDockingOptions();
    void initToolbars();
};
