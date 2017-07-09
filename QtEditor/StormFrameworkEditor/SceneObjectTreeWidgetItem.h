#pragma once
#include <QTreeWidgetItem>

class StormSceneObject;

class SceneObjectTreeWidgetItem : public QTreeWidgetItem {
public:
    SceneObjectTreeWidgetItem(QTreeWidget* parent = nullptr);

    void setSceneObject(StormSceneObject* obj);
    StormSceneObject* getSceneObject();

private:
    StormSceneObject* _SceneObject;
};
