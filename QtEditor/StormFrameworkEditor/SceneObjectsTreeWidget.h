#pragma once
#include <QTreeWidget>
#include <QLayout>

class StormScene;
class StormSceneObject;
class SceneObjectTreeWidgetItem;
class QPlainTextEdit;

class SceneObjectsTreeWidget : public QTreeWidget {
    Q_OBJECT

public:
    SceneObjectsTreeWidget(QWidget* parent = nullptr);
    virtual ~SceneObjectsTreeWidget();

    void populateSceneElements(StormScene* scene);

public slots:
    /* Slot triggered when changing name of obect in text field */
    void objectRenamed();

protected:
    virtual void selectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

private:
    /* Currently active scene */
    StormScene* _Scene;

    /* Pointer to compoents widget, for easy access */
    QWidget* _ObjectComponentsWidget;

    /* Vector of all tree item widgets representing planes */
    std::vector<SceneObjectTreeWidgetItem*> _TreeItemWidgets;

    /* Editable text box for widget name */
    QPlainTextEdit* _ObjectNameTextWidget;

    /* Currently selected item from scene objects tree widget */
    SceneObjectTreeWidgetItem* _SelectedItem;

    /* Adds new scene object and all of its children SceneObjectsTreeWidget as items.
     * Returns pointer to created object. */
    SceneObjectTreeWidgetItem* createSceneObjectListItem(StormSceneObject* object);

    /* Generate widgets for all components in this object */
    void generateComponentWidgets(StormSceneObject* object);

    /* Clear all storm component widgets from @_ObjectComponentsWidgetLayout layout. */
    void clearAllComponentWidgets();

    virtual void dropEvent(QDropEvent* event);
};

