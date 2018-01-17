#pragma once
#include <QTreeWidgetItem>

class SSceneObject;

class SESceneObjectTreeItem : public QTreeWidgetItem {
public:
    SESceneObjectTreeItem(QTreeWidget* parent, SSceneObject* object);
    virtual ~SESceneObjectTreeItem();

    SSceneObject* getSceneObject();

private:
    SSceneObject* _SceneObject;
};
