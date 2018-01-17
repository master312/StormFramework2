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

protected:
    virtual void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

private:
    std::vector<SESceneObjectTreeItem*> _TreeItems;

    void generateSceneObjectItem(SSceneObject* object);
    virtual void dropEvent(QDropEvent* event);
};