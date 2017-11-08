#include "SWidgetComTransform.h"
#include "../propertyWidgets/SWidgetPropertyVec2.h"
#include "../propertyWidgets/SWidgetPropertyFloat.h"
#include "../../src/StormEngine.h"
#include "../../src/scene/components/SSceneComPhysics.h"
#include "../../src/scene/components/SSceneComTransform.h"
#include "../../src/scene/StormSceneObject.h"
#include <QLayout>

SWidgetComTransform::SWidgetComTransform(QWidget* parent /* = nullptr */) : SWidgetComponent(parent) {
    _ComTransform = nullptr;
}

void SWidgetComTransform::initialize() {
    SWidgetComponent::initialize();

    if (!_SceneObject) {
        _HeaderButton->setText("ERROR: Scene object is nullptr!");
        LOG(ERROR) << "Pointer to scene object is nullptr";
        return;
    }
    _ComTransform = _SceneObject->getTransform();
    if (!_ComTransform) {
        return;
    }

    _HeaderButton->setText("Transform");

    /* Create position widget */
    SWidgetPropertyVec2* positionWidget = new SWidgetPropertyVec2(this, "Position");
    positionWidget->setVectorGetter([=]() { return _ComTransform->getPosition(); });
    positionWidget->setVectorSetter([=](Vector2 pos) { _ComTransform->setPosition(pos); });
    layout()->addWidget(positionWidget);

    /* Create scale widget */
    SWidgetPropertyVec2* scaleWidget = new SWidgetPropertyVec2(this, "Scale");
    scaleWidget->setDragFactor(0.05f);
    scaleWidget->setVectorGetter(std::bind(&SSceneComTransform::getScale, _ComTransform));
    scaleWidget->setVectorSetter(std::bind(&SSceneComTransform::setScale, _ComTransform, std::placeholders::_1));
    layout()->addWidget(scaleWidget);

    /* Create angle widget */
    SWidgetPropertyFloat* angleWidget = new SWidgetPropertyFloat(this, "Rotation");
    angleWidget->setDragFactor(0.5f);
    angleWidget->setGetter(std::bind(&SSceneComTransform::getAngle, _ComTransform));
    angleWidget->setSetter(std::bind(&SSceneComTransform::setAngle, _ComTransform, std::placeholders::_1));
    layout()->addWidget(angleWidget);


    foreach (SWidgetProperty* child, findChildren<SWidgetProperty*>()) {
        child->refresh();
    }
}

void SWidgetComTransform::refresh() {
}
