#pragma once
#include <QTreeWidget>
#include <QLayout>

class StormScene;
class StormSceneObject;

class SceneObjectsTreeWidget : public QTreeWidget {
public:
    SceneObjectsTreeWidget(QWidget* parent = nullptr);
    virtual ~SceneObjectsTreeWidget();

    void populateSceneElements(StormScene* scene);

protected:
    virtual void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

private:
    /* Currently active scene */
    StormScene* _Scene;

    /* Pointer to compoents widget, for easy access */
    QWidget* _ObjectComponentsWidget;

    /* Adds new scene object and all of its children SceneObjectsTreeWidget as items. */
    void createSceneObjectListItem(StormSceneObject* object);

    /* Generate widgets for all components in this object */
    void generateComponentWidgets(StormSceneObject* object);

    /* Clear all storm component widgets from @_ObjectComponentsWidgetLayout layout. */
    void clearAllComponentWidgets();

    virtual void dropEvent(QDropEvent* event);
};

