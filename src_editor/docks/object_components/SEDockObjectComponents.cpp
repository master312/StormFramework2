#include "SEDockObjectComponents.h"
#include "scene/SSceneObject.h"
#include "utils/math/Vector2.h"
#include "StormEngineEditing.h"
#include "../../MainWindow.h"
#include "../object_hierarchy/SEDockObjectHierarchy.h"
#include "component_widgets/SERootComponentWidget.h"
#include "property_widgets/SEPropertyVector2.h"
#include <QScrollArea>

SEDockObjectComponents::SEDockObjectComponents(QMainWindow* parent) : SEDockWidget(parent, "Objects Components") {
    QLayout* vLayout = SEDockWidget::genericBoxLayout(_RootWidget);
    QScrollArea* scrollArea = new QScrollArea(_RootWidget);

    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setAlignment(Qt::AlignmentFlag::AlignCenter);
    scrollArea->setWidgetResizable(true);

    QVBoxLayout* scrollAreaVLayout = new QVBoxLayout();
    scrollAreaVLayout->setSizeConstraint(QLayout::SetFixedSize);
    scrollAreaVLayout->setSpacing(1);
    scrollAreaVLayout->setMargin(0);
    scrollAreaVLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    _ScrollArea = new QWidget(this);
    _ScrollArea->setLayout(scrollAreaVLayout);

    scrollArea->setWidget(_ScrollArea);

    vLayout->addWidget(scrollArea);

    S_ADD_GLOBAL_NOTIFICATION_LISTENER(SNotificationType::EDITOR_SCENE_OBJECT_SELECTED, this, SEDockObjectComponents::sceneObjectSelected);
}

SEDockObjectComponents::~SEDockObjectComponents() {
    S_REMOVE_GLOBAL_NOTIFICATION_LISTENER(this);
}

void SEDockObjectComponents::sceneObjectSelected(void* sceneObject) {
    clearGeneratedWidgets();

    SSceneObject* object = static_cast<SSceneObject*>(sceneObject);
    if (!object) {
        /* Object deselected */
        return;
    }
    for (SSceneComponent* component : object->getComponents()) {
        SERootComponentWidget* componentWidget = new SERootComponentWidget(this);

        if (componentWidget->loadComponent(component) < 0) {
            LOG(ERROR) << "Could load scene object component into widget.";
            delete componentWidget;
            componentWidget = nullptr;
            continue;
        }
        if (componentWidget->initialize() < 0) {
            LOG(ERROR) << "Could initialize scene object component widget.";
            delete componentWidget;
            componentWidget = nullptr;
            continue;
        }

        _GeneratedWidgets.push_back(componentWidget);
        _ScrollArea->layout()->addWidget(componentWidget);
        LOG(DEBUG) << "Component type " << (int)component->getType() << " widget generated";
    }
}

void SEDockObjectComponents::clearGeneratedWidgets() {
    for (SERootComponentWidget* widget : _GeneratedWidgets) {
        _ScrollArea->layout()->removeWidget(widget);
        widget->setParent(nullptr);
        widget->close();
        delete widget;
    }
    _GeneratedWidgets.clear();
}