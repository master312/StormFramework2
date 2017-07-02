#include "SceneObjectsTreeWidget.h"
#include "../../src/scene/StormScene.h"
#include <QApplication>
#include <QTreeWidgetItem>
#include <QDropEvent>
#include <QModelIndex>

SceneObjectsTreeWidget::SceneObjectsTreeWidget(QWidget* parent) : QTreeWidget(parent) {
    _Scene = nullptr;

    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragEnabled(true);
    viewport()->setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::InternalMove);
}

SceneObjectsTreeWidget::~SceneObjectsTreeWidget() {
}

void SceneObjectsTreeWidget::populateSceneElements(StormScene* scene) {
    if (_Scene) {
        /* If some scene already exists, clear it's element first */
        QList<QWidget*> widgets = findChildren<QWidget*>();
        foreach(QWidget* widget, widgets) {
            delete widget;
        }
    }
    _Scene = scene;

    for (StormSceneObject* object : _Scene->getObjects()) {
        createSceneObjectListItem(object);
    }
}

void SceneObjectsTreeWidget::createSceneObjectListItem(StormSceneObject* object) {
    QTreeWidgetItem* objectItem = new QTreeWidgetItem(this);
    if (object->getName().size() > 0) {
        /* Object has name specefied */
        objectItem->setText(0, object->getName().c_str());
    } else {
        /* Object dose not have name specified */
        objectItem->setText(0, "Object ID: " + QString::number(object->getId()));
    }
    objectItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
}

void SceneObjectsTreeWidget::dropEvent(QDropEvent* event) {
    QModelIndex droppedIndex = indexAt(event->pos());

    if(!droppedIndex.isValid()) {
        return;
    }

    QTreeWidget::dropEvent(event);
}
