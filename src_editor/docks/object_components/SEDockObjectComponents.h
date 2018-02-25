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

    /* callback for "SNotificationType::SCENE_OBJECT_ADDED" notification.
     * Executed every time object is selected in hierarchy */
    void sceneObjectSelected(void* data);

private:
    QWidget* _ScrollArea;

    /* Vector to keep track of all generated component widgets */
    std::vector<SERootComponentWidget*> _GeneratedWidgets;

    /* Clears all already generated widgets*/
    void clearGeneratedWidgets();
};
