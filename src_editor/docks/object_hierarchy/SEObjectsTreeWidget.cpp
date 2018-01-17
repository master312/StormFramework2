#include "SEObjectsTreeWidget.h"
#include "SESceneObjectTreeItem.h"
#include "scene/SScene.h"

SEObjectsTreeWidget::SEObjectsTreeWidget(QWidget* parent) : QTreeWidget(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(parent->width(), 200);
    setMaximumSize(2000, 2000);

    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);
}

SEObjectsTreeWidget::~SEObjectsTreeWidget() {
}

void SEObjectsTreeWidget::populateList(SScene* scene) {
    std::vector<SSceneObject*>& sceneObjects = scene->getObjects();

    for (size_t i = 0; i < sceneObjects.size(); i++) {
        generateSceneObjectItem(sceneObjects[i]);
    }
}

void SEObjectsTreeWidget::generateSceneObjectItem(SSceneObject* object) {
    SESceneObjectTreeItem* item = new SESceneObjectTreeItem(this, object);
    insertTopLevelItem(0, item);
    _TreeItems.push_back(item);
}

void SEObjectsTreeWidget::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) {

}

void SEObjectsTreeWidget::dropEvent(QDropEvent* event) {

}