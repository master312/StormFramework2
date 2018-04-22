#pragma once
#include "../SEDockWidget.h"
#include "SEObjectsTreeWidget.h"
#include "utils/SEventDispatcher.h"

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

    /* Callback for 'scene about to change' event fired from event manager */
    void cbSceneAboutToChange(const SEventDispatcher::Event* event);

protected:
    /* Called from tree widget every time object is clicked in hierarchy */
    void cbObjectSelected(SSceneObject* selectedObject);

private:
    SEObjectsTreeWidget* _ObjectsTree;
    SSceneObject* _SelectedObject;

    /* Pointer to currently active scene for easy access */
    SScene* _Scene;
};