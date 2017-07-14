#include "SSceneComTransform.h"
#include "../StormSceneObject.h"
#include "../../core/utils/math/ScalarMath.h"

SSceneComTransform::SSceneComTransform(StormSceneObject* owner) : SSceneComponent(owner) {
    _Type = S_SCENE_OBJECT_COM_TRANSFORM;

    _ParentTransform = nullptr;    
    _Position.set(0.0f, 0.0f);
    _PositionAbs.set(0.0f, 0.0f);
    _Scale.set(1.0f, 1.0f);
    _Angle = 0.0f;
    _IsChanged = false;
}

SSceneComTransform::~SSceneComTransform() {
    _ParentTransform = nullptr;
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
    pullParentTransform();
    transform();
    
    S_OBSERVER_ADD(_Owner, this, S_OBSERVER_EVENT_PARENT_CHANGED, SSceneComTransform::observeParentChanged);
    S_OBSERVER_ADD(_Owner, this, S_OBSERVER_EVENT_PARENT_TRANSFORM_UPDATED, SSceneComTransform::observeParentTransformChanged);
}

void SSceneComTransform::transform() {
    if (!_IsChanged) {
        return;
    }
    
    if (!_ParentTransform) {
        /* Component dose not have parent set */
        _PositionAbs = _Position;
    } else {
        /* Parent exists */
        if (_ParentTransform->isChanged()) {
            /* Parent is changed and needs to be transformed first to make sure our position is correct */
            _ParentTransform->transform();
        }

        float sin = StormScalarMath::sin((_ParentTransform->getAngle() * MATH_PI) / 180.0f);
        float cos = StormScalarMath::cos((_ParentTransform->getAngle() * MATH_PI) / 180.0f);
        
        _PositionAbs.x = (_Position.x * cos - _Position.y * sin);
        _PositionAbs.y = (_Position.y * cos + _Position.x * sin);
        _PositionAbs += _ParentTransform->getPositionAbs();
    }
    
    _IsChanged = false;
    /* Notify other components that transformation has been updated */
    _Owner->notifyObservers(S_OBSERVER_EVENT_TRANSFORM_UPDATED);

    for (auto* child : _Owner->getChildren()) {
        /* Notify all children that parent's transformation has been updated */
        child->notifyObservers(S_OBSERVER_EVENT_PARENT_TRANSFORM_UPDATED);
    }
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
    StormScalarMath::clampAngle(&_Angle);
    _IsChanged = true;
}

bool SSceneComTransform::isChanged() {
    return _IsChanged;
}

void SSceneComTransform::observeParentChanged(void* data) {
    pullParentTransform();
}

void SSceneComTransform::observeParentTransformChanged(void* data) {
    _IsChanged = true;
}

void SSceneComTransform::pullParentTransform() {
    if (!_Owner->getParent()) {
        /* Parent has been cleared */
        if (_ParentTransform) {
            _ParentTransform = nullptr;
            _IsChanged = true;
        }
        return;
    }

    SSceneComponent* parentTransform = _Owner->getParent()->getComponent(S_SCENE_OBJECT_COM_TRANSFORM);
    _ParentTransform = dynamic_cast<SSceneComTransform*>(parentTransform);
    _IsChanged = true;
}