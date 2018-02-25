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