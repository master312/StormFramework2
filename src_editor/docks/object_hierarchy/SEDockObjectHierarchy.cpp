#include "SEDockObjectHierarchy.h"
#include <QVBoxLayout>
#include "scene/SSceneObject.h"
#include "StormEngine.h"

SEDockObjectHierarchy::SEDockObjectHierarchy(QMainWindow* parent) : SEDockWidget(parent, "Objects Hierarchy") {

    QLayout* layout = SEDockWidget::genericBoxLayout();
    _RootWidget->setLayout(layout);

    _ObjectsTree = new SEObjectsTreeWidget(this);
    layout->addWidget(_ObjectsTree);

    _SelectedObject = nullptr;
}

SEDockObjectHierarchy::~SEDockObjectHierarchy() {
    S_REMOVE_GLOBAL_NOTIFICATION_LISTENER(_ObjectsTree);
}

void SEDockObjectHierarchy::cbObjectSelected(SSceneObject* selectedObject) {
    if (_SelectedObject == selectedObject) {
        return;
    }
    _SelectedObject = selectedObject;
    sceneObjectSelected(_SelectedObject);
}

void SEDockObjectHierarchy::setScene(SScene* scene) {
    S_ADD_GLOBAL_NOTIFICATION_LISTENER(SNotificationType::SCENE_OBJECT_ADDED, _ObjectsTree, SEObjectsTreeWidget::newPrefabInstantiated);
    _ObjectsTree->populateList(scene);
}

SSceneObject* SEDockObjectHierarchy::getSelectedObject() {
    return _SelectedObject;
}