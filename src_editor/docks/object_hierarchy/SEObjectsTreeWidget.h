#pragma once
#include <QTreeWidget>

class SScene;
class SSceneObject;
class SESceneObjectTreeItem;
class SEDockObjectHierarchy;

class SEObjectsTreeWidget : public QTreeWidget {
    friend class SEDockObjectHierarchy;
    Q_OBJECT

public:
    SEObjectsTreeWidget(SEDockObjectHierarchy* parent);
    virtual ~SEObjectsTreeWidget();

    /* Populate list with scene objects hierarchy from @scene */
    void populateList(SScene* scene);

    /* Removes all script objects from hierarchy list */
    void clearList();

    /* Called when new prefab gets instantiated. @object is pointer to newly instaniated SSceneObject */
    void newSceneObjectAdded(void *object);

public slots:
    void objectSelected(QTreeWidgetItem *item, int column);

protected:
    virtual void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

private:
    SEDockObjectHierarchy* parentDock;

    std::vector<SESceneObjectTreeItem*> _TreeItems;

    /* Generate tree widget list item for @object and all children */
    SESceneObjectTreeItem* generateSceneObjectItem(SSceneObject* object);
    virtual void dropEvent(QDropEvent* event);
};