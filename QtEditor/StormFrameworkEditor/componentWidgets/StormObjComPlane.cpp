#include "StormObjComPlane.h"
#include "../../src/core/StormCommon.h"
#include "../propertyWidgets/StormPropertyVec2Widget.h"
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

    StormPropertyVec2Widget* positionVec = new StormPropertyVec2Widget(this, "Obo je cemg");
    positionVec->setVectorGetter(std::bind(&SSceneComPlane::getPosition, _PlaneComponent));
    positionVec->setVectorSetter(std::bind(&SSceneComPlane::setPosition, _PlaneComponent, std::placeholders::_1));
    layout()->addWidget(positionVec);


    foreach (StormObjComPropertyWidget* child, findChildren<StormObjComPropertyWidget*>()) {
        child->refresh();
    }
}

void StormObjComPlane::refresh() {
    _PlaneComponent->transform();
}
