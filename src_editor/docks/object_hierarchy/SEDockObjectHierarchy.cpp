#include "SEDockObjectHierarchy.h"
#include <QVBoxLayout>
#include "scene/SScene.h"
#include "scene/SSceneObject.h"
#include "scene_editing/lua_script/SESystemLuaScript.h"
#include "StormEngine.h"

SEDockObjectHierarchy::SEDockObjectHierarchy(QMainWindow* parent) : SEDockWidget(parent, "Objects Hierarchy") {

    QLayout* layout = SEDockWidget::genericBoxLayout();
    _RootWidget->setLayout(layout);

    _ObjectsTree = new SEObjectsTreeWidget(this);
    layout->addWidget(_ObjectsTree);

    _SelectedObject = nullptr;

    S_ADD_GLOBAL_NOTIFICATION_LISTENER(SNotificationType::SCENE_MANAGER_SCENE_ABOUT_TO_CHANGE, this, SEDockObjectHierarchy::cbSceneAboutToChange);
}

SEDockObjectHierarchy::~SEDockObjectHierarchy() {
}

void SEDockObjectHierarchy::cbObjectSelected(SSceneObject* selectedObject) {
    if (_SelectedObject == selectedObject) {
        return;
    }
    _SelectedObject = selectedObject;

    S_FIRE_GLOBAL_NOTIFICATION(SNotificationType::EDITOR_SCENE_OBJECT_SELECTED, _SelectedObject);
}

void SEDockObjectHierarchy::setScene(SScene* scene) {
    _ObjectsTree->populateList(scene);
    _Scene = scene;
}

SScene* SEDockObjectHierarchy::getScene() {
    return _Scene;
}

SSceneObject* SEDockObjectHierarchy::getSelectedObject() {
    return _SelectedObject;
}

void SEDockObjectHierarchy::cbSceneAboutToChange(void* scene) {
    if (static_cast<void*>(_Scene) == scene) {
        return;
    }
    /* Deselect object */
    cbObjectSelected(nullptr);

    /* Destroy scene and hierarchy tree items */
    _Scene = nullptr;
    _ObjectsTree->clearList();
    if (!scene) {
        LOG(WARNING) << "Scene is about to change, but next scene is nullptr";
        return;
    }
    SScene* sScene = static_cast<SScene*>(scene);
    setScene(sScene);
}