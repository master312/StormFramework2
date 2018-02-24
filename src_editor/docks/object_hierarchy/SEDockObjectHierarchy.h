#pragma once
#include "../SEDockWidget.h"
#include "SEObjectsTreeWidget.h"

class SScene;

class SEDockObjectHierarchy : public SEDockWidget {
    friend class SEObjectsTreeWidget;
    Q_OBJECT

public:
    SEDockObjectHierarchy(QMainWindow* parent);
    virtual ~SEDockObjectHierarchy();

    void setScene(SScene* scene);

    /* Returns currently active scene */
    SScene* getScene();

    SSceneObject* getSelectedObject();

signals:
    /* Fired every time new object has been selected */
    void sceneObjectSelected(SSceneObject* selectedObject);

protected:
    /* Called from tree widget every time object is clicked in hierarchy */
    void cbObjectSelected(SSceneObject* selectedObject);

private:
    SEObjectsTreeWidget* _ObjectsTree;
    SSceneObject* _SelectedObject;

    /* Pointer to currently active scene for easy access */
    SScene* _Scene;
};