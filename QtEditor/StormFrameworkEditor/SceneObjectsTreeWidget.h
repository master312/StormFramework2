#pragma once
#include <QTreeWidget>

class StormScene;
class StormSceneObject;

class SceneObjectsTreeWidget : public QTreeWidget {
public:
    SceneObjectsTreeWidget(QWidget* parent = 0);
    virtual ~SceneObjectsTreeWidget();

    void populateSceneElements(StormScene* scene);

protected:
    virtual void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

private:
    /* Currently active scene */
    StormScene* _Scene;

    /* Adds new scene object and all of its children SceneObjectsTreeWidget as items. */
    void createSceneObjectListItem(StormSceneObject* object);

    virtual void dropEvent(QDropEvent* event);
};

