#pragma once
#include "../SEDockWidget.h"

class SEStormMainWidget;

class SEMainEditorDock : public SEDockWidget {
    Q_OBJECT
public:
    SEMainEditorDock(QMainWindow* parent);
    virtual ~SEMainEditorDock();

    SEStormMainWidget* getMainEngineWidget();

private:
    SEStormMainWidget* _MainWidget;
};