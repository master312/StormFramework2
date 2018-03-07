#include "SEObjectsTreeWidget.h"
#include "SESceneObjectTreeItem.h"
#include "StormEngine.h"
#include "scene/SScene.h"
#include "SEDockObjectHierarchy.h"
#include <QDropEvent>

SEObjectsTreeWidget::SEObjectsTreeWidget(SEDockObjectHierarchy* parent) : QTreeWidget(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setMinimumSize(parent->width(), 200);
    setMaximumSize(2000, 2000);
    parentDock = parent;

    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);

    connect(this, &SEObjectsTreeWidget::itemClicked, this, &SEObjectsTreeWidget::objectSelected);
    S_ADD_GLOBAL_NOTIFICATION_LISTENER(SNotificationType::SCENE_OBJECT_ADDED, this, SEObjectsTreeWidget::newSceneObjectAdded);
}

SEObjectsTreeWidget::~SEObjectsTreeWidget() {
    S_REMOVE_GLOBAL_NOTIFICATION_LISTENER(this);
}

void SEObjectsTreeWidget::newSceneObjectAdded(void* object) {
    SSceneObject* sceneObject = static_cast<SSceneObject*>(object);
    if (!sceneObject) {
        LOG(WARNING) << "Could not cast void* ptr to SSceneObject";
        return;
    }

    generateSceneObjectItem(sceneObject);
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

void SEObjectsTreeWidget::clearList() {
    QTreeWidgetItem* item = takeTopLevelItem(0);
    while(item) {
        delete item;
        item = takeTopLevelItem(0);
    }
    _TreeItems.clear();
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

void SEObjectsTreeWidget::objectSelected(QTreeWidgetItem* item, int column) {
    if (!item) {
        return;
    }
    SESceneObjectTreeItem* treeItem = dynamic_cast<SESceneObjectTreeItem*>(item);
    if (!treeItem) {
        return;
    }
    parentDock->cbObjectSelected(treeItem->getSceneObject());
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