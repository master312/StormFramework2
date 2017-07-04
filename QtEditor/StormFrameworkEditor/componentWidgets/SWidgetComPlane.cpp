#include "SWidgetComPlane.h"
#include "../../src/core/StormCommon.h"
#include "../../src/scene/components/SSceneComPlane.h"
#include "../propertyWidgets/SWidgetPropertyVec2.h"
#include "../propertyWidgets/SWidgetPropertyFloat.h"
#include <QLayout>

SWidgetComPlane::SWidgetComPlane(QWidget* parent) : SWidgetComponent(parent) {
    _PlaneComponent = nullptr;
}

void SWidgetComPlane::initialize() {
    SWidgetComponent::initialize();
    _PlaneComponent = dynamic_cast<SSceneComPlane*>(_StormComponent);
    if (!_PlaneComponent) {
        LOG(ERROR) << "StormObjComPlane::initialize null plane component";
        return;
    }

    /* Create position widget */
    SWidgetPropertyVec2* positionWidget = new SWidgetPropertyVec2(this, "Position");
    positionWidget->setVectorGetter(std::bind(&SSceneComPlane::getPosition, _PlaneComponent));
    positionWidget->setVectorSetter(std::bind(&SSceneComPlane::setPosition, _PlaneComponent, std::placeholders::_1));
    layout()->addWidget(positionWidget);

    /* Create size widget */
    SWidgetPropertyVec2* sizeWidget = new SWidgetPropertyVec2(this, "Size");
    sizeWidget->setVectorGetter(std::bind(&SSceneComPlane::getSize, _PlaneComponent));
    sizeWidget->setVectorSetter(std::bind(&SSceneComPlane::setSize, _PlaneComponent, std::placeholders::_1));
    layout()->addWidget(sizeWidget);

    /* Create scale widget */
    SWidgetPropertyVec2* scaleWidget = new SWidgetPropertyVec2(this, "Scale");
    scaleWidget->setDragFactor(0.05f);
    scaleWidget->setVectorGetter(std::bind(&SSceneComPlane::getScale, _PlaneComponent));
    scaleWidget->setVectorSetter(std::bind(&SSceneComPlane::setScale, _PlaneComponent, std::placeholders::_1));
    layout()->addWidget(scaleWidget);

    /* Create angle widget */
    SWidgetPropertyFloat* angleWidget = new SWidgetPropertyFloat(this, "Rotation");
    angleWidget->setDragFactor(0.5f);
    angleWidget->setGetter(std::bind(&SSceneComPlane::getAngle, _PlaneComponent));
    angleWidget->setSetter(std::bind(&SSceneComPlane::setAngle, _PlaneComponent, std::placeholders::_1));
    layout()->addWidget(angleWidget);

    foreach (SWidgetProperty* child, findChildren<SWidgetProperty*>()) {
        child->refresh();
    }
}

void SWidgetComPlane::refresh() {
    _PlaneComponent->transform();
}
