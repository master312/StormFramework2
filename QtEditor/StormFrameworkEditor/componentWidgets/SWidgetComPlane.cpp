#include "SWidgetComPlane.h"
#include "../../src/core/StormCommon.h"
#include "../../src/scene/components/SSceneComPlane.h"
#include "../propertyWidgets/SWidgetPropertyVec2.h"
#include "../propertyWidgets/SWidgetPropertyBoolean.h"
#include <QLayout>

SWidgetComPlane::SWidgetComPlane(QWidget* parent) : SWidgetComponent(parent) {
    _PlaneComponent = nullptr;
}

SWidgetComPlane::~SWidgetComPlane() {
    if (_PlaneComponent) {
        _PlaneComponent->setRenderDebug(false);
    }
}

void SWidgetComPlane::initialize() {
    SWidgetComponent::initialize();
    _PlaneComponent = dynamic_cast<SSceneComPlane*>(_StormComponent);
    if (!_PlaneComponent) {
        LOG(ERROR) << "StormObjComPlane::initialize null plane component";
        return;
    }

    /* Create size widget */
    SWidgetPropertyVec2* sizeWidget = new SWidgetPropertyVec2(this, "Size");
    sizeWidget->setVectorGetter(std::bind(&SSceneComPlane::getSize, _PlaneComponent));
    sizeWidget->setVectorSetter(std::bind(&SSceneComPlane::setSize, _PlaneComponent, std::placeholders::_1));
    layout()->addWidget(sizeWidget);

    /* Create inherit rotation widget */
    SWidgetPropertyBoolean* inheritRotationWidget = new SWidgetPropertyBoolean(this, "Inherit rotation");
    inheritRotationWidget->setGetter(std::bind(&SSceneComPlane::getInheritRotation, _PlaneComponent));
    inheritRotationWidget->setSetter(std::bind(&SSceneComPlane::setInheritRotation, _PlaneComponent, std::placeholders::_1));
    layout()->addWidget(inheritRotationWidget);

    /* Create inherit scale widget */
    SWidgetPropertyBoolean* inheritScaleWidget = new SWidgetPropertyBoolean(this, "Inherit scale");
    inheritScaleWidget->setGetter(std::bind(&SSceneComPlane::getInheritScale, _PlaneComponent));
    inheritScaleWidget->setSetter(std::bind(&SSceneComPlane::setInheritScale, _PlaneComponent, std::placeholders::_1));
    layout()->addWidget(inheritScaleWidget);

    foreach (SWidgetProperty* child, findChildren<SWidgetProperty*>()) {
        child->refresh();
    }

    _PlaneComponent->setRenderDebug(true);
}

void SWidgetComPlane::refresh() {
    //_PlaneComponent->transform();
}
