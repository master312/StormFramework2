#pragma once
#include "../SEDockWidget.h"

class SEStormMainWidget;

class SEMainEditorDock : public SEDockWidget {
    Q_OBJECT
public:
    SEMainEditorDock(QMainWindow* parent);
    virtual ~SEMainEditorDock();

private:
    SEStormMainWidget* _MainWidget;
};