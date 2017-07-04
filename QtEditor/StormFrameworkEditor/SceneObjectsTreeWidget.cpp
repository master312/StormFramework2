#include "SceneObjectsTreeWidget.h"
#include "../../src/scene/StormScene.h"
#include "../../src/scene/SSceneComponent.h"
#include "StormObjComponentWidget.h"
#include "StormQtHelperFunctions.h"
#include <QApplication>
#include <QTreeWidgetItem>
#include <QDropEvent>
#include <QModelIndex>
#include <QApplication>

SceneObjectTreeWidgetItem::SceneObjectTreeWidgetItem(QTreeWidget* parent) : QTreeWidgetItem(parent) {
    _SceneObject = nullptr;
}

SceneObjectTreeWidgetItem::~SceneObjectTreeWidgetItem() {
}

void SceneObjectTreeWidgetItem::setSceneObject(StormSceneObject* obj) {
    _SceneObject = obj;
}

StormSceneObject* SceneObjectTreeWidgetItem::getSceneObject() {
    return _SceneObject;
}





SceneObjectsTreeWidget::SceneObjectsTreeWidget(QWidget* parent) : QTreeWidget(parent) {
    _Scene = nullptr;

    setSelectionMode(QAbstractItemView::ExtendedSelection);
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


    /* Find components widget */
    _ObjectComponentsWidgetLayout = StormQtHelper::findChildByName(parentWidget(), "objectComponents")->layout();


    for (StormSceneObject* object : _Scene->getObjects()) {
        createSceneObjectListItem(object);
    }
}

void SceneObjectsTreeWidget::selectionChanged(const QItemSelection& selected, const QItemSelection& deselected) {
    /* Repaint just to prevent blinking while selecting on linux */
    foreach (QWidget* child, findChildren<QWidget*>()) {
        child->repaint();
    }

    if (selectedItems().size() != 1) {
        /* Dont show components if multiple objects are selected */
        clearAllComponentWidgets();
        return;
    }

    SceneObjectTreeWidgetItem* selectedItem = dynamic_cast<SceneObjectTreeWidgetItem*>(selectedItems()[0]);
    if (!selectedItem || !selectedItem->getSceneObject()) {
        LOG(ERROR) << "SceneObjectsTreeWidget::selectionChanged No selected item or invalid item type";
        return;
    }

    /* Destroy all component widgets */
    clearAllComponentWidgets();

    /* And create new onse */
    generateComponentWidgets(selectedItem->getSceneObject());
}

void SceneObjectsTreeWidget::createSceneObjectListItem(StormSceneObject* object) {
    SceneObjectTreeWidgetItem* objectItem = new SceneObjectTreeWidgetItem(this);
    objectItem->setSceneObject(object);
    if (object->getName().size() > 0) {
        /* Object has name specefied */
        objectItem->setText(0, object->getName().c_str());
    } else {
        /* Object dose not have name specified */
        objectItem->setText(0, "Object ID: " + QString::number(object->getId()));
    }
    objectItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
}

void SceneObjectsTreeWidget::generateComponentWidgets(StormSceneObject* object) {
    for (SSceneComponent* component : object->getComponents()) {
        StormObjComponentWidget* comWidget = StormObjComponentWidget::newWidget(component->getType(), component);
        if (!comWidget) {
            LOG(ERROR) << "Missing QT widget for component " << SSceneComponentTypeString[component->getType()];
            continue;
        }
        comWidget->setParent(_ObjectComponentsWidgetLayout->widget());
        comWidget->initialize();
        _ObjectComponentsWidgetLayout->addWidget(comWidget);
    }
}

void SceneObjectsTreeWidget::clearAllComponentWidgets() {
    for (int i = 0; i < _ObjectComponentsWidgetLayout->count(); i++) {
        QLayoutItem* item = _ObjectComponentsWidgetLayout->itemAt(i);
        if (item && item->widget()) {
            delete item->widget();
        }
    }
}

void SceneObjectsTreeWidget::dropEvent(QDropEvent* event) {
    QModelIndex droppedIndex = indexAt(event->pos());

    if(!droppedIndex.isValid()) {
        return;
    }

    QTreeWidget::dropEvent(event);
}
