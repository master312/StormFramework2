#include "StormObjComPlane.h"
#include "../../src/core/StormCommon.h"
#include "../propertyWidgets/StormPropertyVec2Widget.h"
#include "../propertyWidgets/SWidgetPropertyFloat.h"
#include <QLayout>

StormObjComPlane::StormObjComPlane(QWidget* parent) : StormObjComponentWidget(parent) {
    _PlaneComponent = nullptr;
}

void StormObjComPlane::initialize() {
    StormObjComponentWidget::initialize();
    _PlaneComponent = dynamic_cast<SSceneComPlane*>(_StormComponent);
    if (!_PlaneComponent) {
        LOG(ERROR) << "StormObjComPlane::initialize null plane component";
        return;
    }

    /* Create position widget */
    StormPropertyVec2Widget* positionWidget = new StormPropertyVec2Widget(this, "Position ");
    positionWidget->setVectorGetter(std::bind(&SSceneComPlane::getPosition, _PlaneComponent));
    positionWidget->setVectorSetter(std::bind(&SSceneComPlane::setPosition, _PlaneComponent, std::placeholders::_1));
    layout()->addWidget(positionWidget);

    /* Create size widget */
    StormPropertyVec2Widget* sizeWidget = new StormPropertyVec2Widget(this, "Size ");
    sizeWidget->setVectorGetter(std::bind(&SSceneComPlane::getSize, _PlaneComponent));
    sizeWidget->setVectorSetter(std::bind(&SSceneComPlane::setSize, _PlaneComponent, std::placeholders::_1));
    layout()->addWidget(sizeWidget);

    /* Create scale widget */
    StormPropertyVec2Widget* scaleWidget = new StormPropertyVec2Widget(this, "Scale ");
    scaleWidget->setDragFactor(0.05f);
    scaleWidget->setVectorGetter(std::bind(&SSceneComPlane::getScale, _PlaneComponent));
    scaleWidget->setVectorSetter(std::bind(&SSceneComPlane::setScale, _PlaneComponent, std::placeholders::_1));
    layout()->addWidget(scaleWidget);

    /* Create angle widget */
    SWidgetPropertyFloat* angleWidget = new SWidgetPropertyFloat(this, "Rotation ");
    angleWidget->setDragFactor(0.5f);
    angleWidget->setGetter(std::bind(&SSceneComPlane::getAngle, _PlaneComponent));
    angleWidget->setSetter(std::bind(&SSceneComPlane::setAngle, _PlaneComponent, std::placeholders::_1));
    layout()->addWidget(angleWidget);

    foreach (StormObjComPropertyWidget* child, findChildren<StormObjComPropertyWidget*>()) {
        child->refresh();
    }
}

void StormObjComPlane::refresh() {
    _PlaneComponent->transform();
}
