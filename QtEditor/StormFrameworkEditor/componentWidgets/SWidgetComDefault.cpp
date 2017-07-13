#include "SWidgetComDefault.h"
#include "../propertyWidgets/SWidgetPropertyVec2.h"
#include "../../src/StormEngine.h"
#include "../../src/scene/components/SSceneComPlane.h"
#include <QLayout>

SWidgetComDefault::SWidgetComDefault(QWidget* parent /* = nullptr */) : SWidgetComponent(parent) {

}

void SWidgetComDefault::initialize() {
    SWidgetComponent::initialize();

    if (!_SceneObject) {
        _HeaderButton->setText("ERROR: Scene object is nullptr!");
        LOG(ERROR) << "Pointer to scene object is nullptr";
        return;
    }
    _HeaderButton->setText("Object settings");

    /* Create position widget */
    SWidgetPropertyVec2* positionWidget = new SWidgetPropertyVec2(this, "Position");
    positionWidget->setVectorGetter(std::bind(&StormSceneObject::getPosition, _SceneObject));
    positionWidget->setVectorSetter(std::bind(&StormSceneObject::setPosition, _SceneObject, std::placeholders::_1));
    layout()->addWidget(positionWidget);


    foreach (SWidgetProperty* child, findChildren<SWidgetProperty*>()) {
        child->refresh();
    }
}

void SWidgetComDefault::refresh() {
    SSceneComPlane* componentPlane = dynamic_cast<SSceneComPlane*>(_SceneObject->getComponent(S_SCENE_OBJECT_COM_PLANE));
    if (componentPlane) {
        componentPlane->transform();
    }
}
