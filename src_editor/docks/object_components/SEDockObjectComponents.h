#pragma once
#include "../SEDockWidget.h"

/* Dock used for managing selected object's components */

class SSceneObject;
class SERootComponentWidget;

class SEDockObjectComponents : public SEDockWidget {
    Q_OBJECT

public:
    SEDockObjectComponents(QMainWindow* parent);
    virtual ~SEDockObjectComponents();

public slots:
    /* Executed every time object is selected in hierarchy */
    void sceneObjectSelected(SSceneObject* object);

private:
    QWidget* _ScrollArea;

    /* Vector to keep track of all generated component widgets */
    std::vector<SERootComponentWidget*> _GeneratedWidgets;

    /* Clears all already generated widgets*/
    void clearGeneratedWidgets();
};
