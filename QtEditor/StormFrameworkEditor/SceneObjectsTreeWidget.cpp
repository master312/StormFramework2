#include "SceneObjectsTreeWidget.h"
#include "SceneObjectTreeWidgetItem.h"
#include "SWidgetComponent.h"
#include "StormQtHelperFunctions.h"
#include "../../src/scene/StormScene.h"
#include "../../src/scene/SSceneComponent.h"
#include <QApplication>
#include <QDropEvent>
#include <QModelIndex>
#include <QApplication>

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
        _TreeItemWidgets.clear();
    }
    _Scene = scene;

    /* Find components widget */
    _ObjectComponentsWidget = StormQtHelper::findChildByName(parentWidget(), "objectComponents");

    for (StormSceneObject* object : _Scene->getObjects()) {
        _TreeItemWidgets.push_back(createSceneObjectListItem(object));
    }

    /* Create children hierarchy */
    /* TODO: Optimize */
    for (SceneObjectTreeWidgetItem* widget : _TreeItemWidgets) {
        StormSceneObject* parent = widget->getSceneObject()->getParent();
        if (!parent) {
            continue;
        }

        for (SceneObjectTreeWidgetItem* parentWidget : _TreeItemWidgets) {
            if (parentWidget->getSceneObject() == parent) {
                parentWidget->addChild(widget);
                break;
            }
        }
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

SceneObjectTreeWidgetItem* SceneObjectsTreeWidget::createSceneObjectListItem(StormSceneObject* object) {
    SceneObjectTreeWidgetItem* objectItem = new SceneObjectTreeWidgetItem(object->getParent() ? nullptr : this);
    objectItem->setSceneObject(object);
    if (object->getName().size() > 0) {
        /* Object has name specefied */
        objectItem->setText(0, object->getName().c_str());
    } else {
        /* Object dose not have name specified */
        objectItem->setText(0, "Object ID: " + QString::number(object->getId()));
    }
    objectItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);

    return objectItem;
}

void SceneObjectsTreeWidget::generateComponentWidgets(StormSceneObject* object) {
    /* First generate default widget component.
     * This component is used for editing common scene object stuff, like position and name. */
    SWidgetComponent* defaultWidget = SWidgetComponent::newWidget(object, nullptr, _ObjectComponentsWidget);
    if (!defaultWidget) {
        LOG(ERROR) << "Failed to create QT widget for default component";
        return;
    }
    defaultWidget->initialize();
    _ObjectComponentsWidget->layout()->addWidget(defaultWidget);

    for (SSceneComponent* component : object->getComponents()) {
        SWidgetComponent* comWidget = SWidgetComponent::newWidget(object, component, _ObjectComponentsWidget);
        if (!comWidget) {
            LOG(ERROR) << "Missing QT widget for component " << SSceneComponentTypeString[component->getType()];
            continue;
        }
        comWidget->initialize();
        _ObjectComponentsWidget->layout()->addWidget(comWidget);
    }
}

void SceneObjectsTreeWidget::clearAllComponentWidgets() {
    for (QWidget* widget : _ObjectComponentsWidget->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly)) {
        delete widget;
    }
}

void SceneObjectsTreeWidget::dropEvent(QDropEvent* event) {
    QModelIndex droppedIndex = indexAt(event->pos());

    if(!droppedIndex.isValid()) {
        return;
    }

    QTreeWidget::dropEvent(event);
    for(int i = 0; i < _TreeItemWidgets.size(); i++) {
        SceneObjectTreeWidgetItem* widget = dynamic_cast<SceneObjectTreeWidgetItem*>(_TreeItemWidgets[i]);
        SceneObjectTreeWidgetItem* parent = dynamic_cast<SceneObjectTreeWidgetItem*>(widget->parent());
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