#pragma once
#include <QTreeWidget>

class SScene;
class SSceneObject;
class SESceneObjectTreeItem;

class SEObjectsTreeWidget : public QTreeWidget {
    Q_OBJECT
public:
    SEObjectsTreeWidget(QWidget* parent);
    virtual ~SEObjectsTreeWidget();

    /* Populate list with scene objects hierarchy from @scene */
    void populateList(SScene* scene);

    /* Called when new prefab gets instantiated. @object is pointer to newly instaniated SSceneObject */
    void newPrefabInstantiated(void* object);

protected:
    virtual void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

private:
    std::vector<SESceneObjectTreeItem*> _TreeItems;

    /* Generate tree widget list item for @object and all children */
    SESceneObjectTreeItem* generateSceneObjectItem(SSceneObject* object);
    virtual void dropEvent(QDropEvent* event);
};