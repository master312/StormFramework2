#include "SSceneComTransform.h"
#include "../StormSceneObject.h"

SSceneComTransform::SSceneComTransform(StormSceneObject* owner) : SSceneComponent(owner) {
    _Type = S_SCENE_OBJECT_COM_TRANSFORM;
    
    _Position.set(0.0f, 0.0f);
    _PositionAbs.set(0.0f, 0.0f);
    _Scale.set(1.0f, 1.0f);
    _Angle = 0.0f;
    _IsChanged = false;
}

SSceneComTransform::~SSceneComTransform() {
}

void SSceneComTransform::serializeXml(pugi::xml_node& node) {
    SSceneComponent::serializeXml(node);

    node.append_attribute("pos_x").set_value(_Position.x);
    node.append_attribute("pos_y").set_value(_Position.y);
    node.append_attribute("scale_x").set_value(_Scale.x);
    node.append_attribute("scale_y").set_value(_Scale.y);
    node.append_attribute("angle").set_value(_Angle);
}

int SSceneComTransform::deserializeXml(pugi::xml_node& node) {
    SSceneComponent::deserializeXml(node);

    _Position.x = node.attribute("pos_x").as_float(0.0f);
    _Position.y = node.attribute("pos_y").as_float(0.0f);
    _Scale.x = node.attribute("scale_x").as_float(0.0f);
    _Scale.y = node.attribute("scale_y").as_float(0.0f);
    _Angle = node.attribute("angle").as_float(0.0f);

    _PositionAbs = _Position;

    return 1;
}

void SSceneComTransform::initialize() {
    transform();
}

void SSceneComTransform::transform() {
    /* TODO: Parent linking calculations hare */
    if (!_IsChanged) {
        return;
    }
    
    _Owner->fireEvent(S_OBSERVER_EVENT_TRANSFORM_UPDATED);
    _PositionAbs = _Position;
    _IsChanged = false;
}

Vector2 SSceneComTransform::getPosition() {
    return _Position;
}

void SSceneComTransform::setPosition(Vector2 position) {
    _Position = position;
    _IsChanged = true;
}

Vector2 SSceneComTransform::getPositionAbs() {
    return _PositionAbs;
}

Vector2 SSceneComTransform::getScale() {
    return _Scale;
}

void SSceneComTransform::setScale(Vector2 scale) {
    _Scale = scale;
    _IsChanged = true;
}

float SSceneComTransform::getAngle() {
    return _Angle;
}

void SSceneComTransform::setAngle(float angle) {
    _Angle = angle;
    _IsChanged = true;
}

bool SSceneComTransform::isChanged() {
    return _IsChanged;
}