#include "SEObjectsTreeWidget.h"
#include "SESceneObjectTreeItem.h"
#include "scene/SScene.h"
#include <QDropEvent>

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
        SSceneObject* object = sceneObjects[i];
        if (object->getParent()) {
            continue;
        }
        generateSceneObjectItem(object);
    }
}

SESceneObjectTreeItem* SEObjectsTreeWidget::generateSceneObjectItem(SSceneObject* object) {
    SESceneObjectTreeItem* item = new SESceneObjectTreeItem(object->getParent() ? nullptr : this, object);
    _TreeItems.push_back(item);

    for (SSceneObject* childObj : object->getChildren()) {
        SESceneObjectTreeItem* childItem = generateSceneObjectItem(childObj);
        item->addChild(childItem);
        _TreeItems.push_back(childItem);
    }

    return item;
}

void SEObjectsTreeWidget::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) {

}

void SEObjectsTreeWidget::dropEvent(QDropEvent* event) {
    QModelIndex droppedIndex = indexAt(event->pos());

    if(!droppedIndex.isValid()) {
        return;
    }

    QTreeWidget::dropEvent(event);
    for(SESceneObjectTreeItem* widget : _TreeItems) {
        SESceneObjectTreeItem* parent = static_cast<SESceneObjectTreeItem*>(widget->parent());
        if (!parent) {
            widget->getSceneObject()->setParent(nullptr);
            continue;
        }
        if (widget->getSceneObject()->getParent() != parent->getSceneObject()) {
            /* Object parent not matching parent in the list */
            widget->getSceneObject()->setParent(parent->getSceneObject());
        }
    }
}