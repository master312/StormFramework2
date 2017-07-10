#include "SceneObjectTreeWidgetItem.h"

SceneObjectTreeWidgetItem::SceneObjectTreeWidgetItem(QTreeWidget* parent) : QTreeWidgetItem(parent) {
    _SceneObject = nullptr;
}

void SceneObjectTreeWidgetItem::setSceneObject(StormSceneObject* obj) {
    _SceneObject = obj;
}

StormSceneObject* SceneObjectTreeWidgetItem::getSceneObject() {
    return _SceneObject;
}
