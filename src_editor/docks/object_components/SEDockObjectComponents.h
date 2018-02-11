#pragma once
#include "../SEDockWidget.h"

/* Dock used for managing selected object's components */

class SSceneObject;

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
};
