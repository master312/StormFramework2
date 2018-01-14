#include "SSceneComTransform.h"
#include "scene/SSceneObject.h"
#include "utils/math/ScalarMath.h"

SSceneComTransform::SSceneComTransform(SSceneObject* owner) : SSceneComponent(owner) {
    _Type = S_SCENE_OBJECT_COM_TRANSFORM;

    _ParentTransform = nullptr;    
    _Position.set(0.0f, 0.0f);
    _PositionAbs.set(0.0f, 0.0f);
    _Scale.set(1.0f, 1.0f);
    _Angle = 0.0f;
    _IsChanged = false;
    _InheritScale = true;
    _ParentAsPivot = false;
    _ScriptHandlerName = "transform";
    _ZPriority = 0;
}

SSceneComTransform::~SSceneComTransform() {
    _ParentTransform = nullptr;
}

void SSceneComTransform::serializeXml(pugi::xml_node& node) {
    SSceneComponent::serializeXml(node);

    node.append_attribute("pos_x").set_value(_Position.x);
    node.append_attribute("pos_y").set_value(_Position.y);
    node.append_attribute("pos_z").set_value(_ZPriority);
    node.append_attribute("scale_x").set_value(_Scale.x);
    node.append_attribute("scale_y").set_value(_Scale.y);
    node.append_attribute("angle").set_value(_Angle);
    node.append_attribute("inherit_scale").set_value(_InheritScale);
    node.append_attribute("parent_as_pivot").set_value(_ParentAsPivot);
}

int SSceneComTransform::deserializeXml(pugi::xml_node& node) {
    SSceneComponent::deserializeXml(node);

    _Position.x = node.attribute("pos_x").as_float(0.0f);
    _Position.y = node.attribute("pos_y").as_float(0.0f);
    _ZPriority = node.attribute("pos_z").as_int(0);
    _Scale.x = node.attribute("scale_x").as_float(0.0f);
    _Scale.y = node.attribute("scale_y").as_float(0.0f);
    _Angle = node.attribute("angle").as_float(0.0f);
    _InheritScale = node.attribute("inherit_scale").as_bool(true);
    _ParentAsPivot = node.attribute("parent_as_pivot").as_bool(false);

    _PositionAbs = _Position;
    _ScaleAbs = _Scale;

    return 1;
}

int SSceneComTransform::initialize(SSceneComponentSystem* system) {
    SSceneComponent::initialize(system);

    pullParentTransform();
    transform();
    
    S_OBSERVER_ADD(_Owner, this, S_OBSERVER_EVENT_PARENT_CHANGED, SSceneComTransform::observeParentChanged);
    S_OBSERVER_ADD(_Owner, this, S_OBSERVER_EVENT_PARENT_TRANSFORM_UPDATED, SSceneComTransform::observeParentTransformChanged);

    return 1;
}

void SSceneComTransform::transform() {
    if (!_IsChanged) {
        return;
    }
    
    if (!_ParentTransform) {
        /* Component dose not have parent set */
        _PositionAbs = _Position;
        _ScaleAbs = _Scale;
    } else {
        /* Parent exists */
        if (_ParentTransform->isChanged()) {
            /* Parent is changed and needs to be transformed first to make sure our position is correct */
            _ParentTransform->transform();
        }

        float sin = StormScalarMath::sin((_ParentTransform->getAngleAbs() * MATH_PI) / 180.0f);
        float cos = StormScalarMath::cos((_ParentTransform->getAngleAbs() * MATH_PI) / 180.0f);
        
        _PositionAbs.x = (_Position.x * cos - _Position.y * sin);
        _PositionAbs.y = (_Position.y * cos + _Position.x * sin);
        _PositionAbs += _ParentTransform->getPositionAbs();

        if (_InheritScale) {
            _ScaleAbs = _Scale.mult(_ParentTransform->getScaleAbs());
        } else {
            _ScaleAbs = _Scale;
        }
    }
    
    /* Notify other components that transformation has been updated */
    _Owner->notifyObservers(S_OBSERVER_EVENT_TRANSFORM_UPDATED);
    
    for (auto* child : _Owner->getChildren()) {
        /* Notify all children that parent's transformation has been updated */
        child->notifyObservers(S_OBSERVER_EVENT_PARENT_TRANSFORM_UPDATED);
    }
    
    _IsChanged = false;
}

Vector2 SSceneComTransform::getPosition() const {
    return _Position;
}

void SSceneComTransform::setPosition(const Vector2 position) {
    setPosition(position.x, position.y);
}

void SSceneComTransform::setPosition(float x, float y) {
    if (StormScalarMath::equivalent(_Position.x, x) &&
        StormScalarMath::equivalent(_Position.y, y)) {
            return;
    }
    _Position.x = x;
    _Position.y = y;
    _IsChanged = true;
}

void SSceneComTransform::setPositionAbs(const Vector2 position) {
    if (!_ParentTransform) {
        setPosition(position);
        return;
    }
    
    setPosition(position - _ParentTransform->getPositionAbs());
}

void SSceneComTransform::setPositionAbs(float x, float y) {
    if (!_ParentTransform) {
        setPosition(x, y);
        return;
    }
    setPosition(x - _ParentTransform->getAbsX(), 
                y - _ParentTransform->getAbsY());
}

void SSceneComTransform::setX(float x) {
    setPosition(x, _Position.y);
}

void SSceneComTransform::setY(float y) {
    setPosition(_Position.x, y);
}

float SSceneComTransform::getX() const {
    return _Position.x;
}

float SSceneComTransform::getY() const {
    return _Position.y;
}

Vector2 SSceneComTransform::getPositionAbs() const {
    return _PositionAbs;
}

float SSceneComTransform::getAbsX() const {
    return _PositionAbs.x;
}

float SSceneComTransform::getAbsY() const {
    return _PositionAbs.y;
}

Vector2 SSceneComTransform::getScale() const {
    return _Scale;
}

Vector2 SSceneComTransform::getScaleAbs() const {
    return _ScaleAbs;
}

void SSceneComTransform::setScale(Vector2 scale) {
    if (_Scale == scale) {
        return;
    }
    _Scale = scale;
    _IsChanged = true;
}

int SSceneComTransform::getZ() const {
    return _ZPriority;
}

int SSceneComTransform::getAbsZ() const {
    if (!_ParentTransform) {
        return _ZPriority;
    }
    return _ZPriority + _ParentTransform->getZ();
}

void SSceneComTransform::setZ(int value) {
    _ZPriority = value;
}

float SSceneComTransform::getAngle() const {
    return _Angle;
}

float SSceneComTransform::getAngleAbs() const {
    if (_ParentTransform) {
        return StormScalarMath::clampAngle(_ParentTransform->getAngleAbs() + _Angle);
    }
    return _Angle;
}

void SSceneComTransform::setAngle(float angle) {
    angle = StormScalarMath::clampAngle(angle);
    if (StormScalarMath::equivalent(angle, _Angle)) {
        return;
    }
    _Angle = angle;
    _IsChanged = true;
}

void SSceneComTransform::setInheritScale(bool value) {
    if (_InheritScale == value) {
        return;
    }
    _InheritScale = value;
    _IsChanged = true;
}

bool SSceneComTransform::getInheritScale() const {
    return _InheritScale;
}

void SSceneComTransform::setParentAsPivot(bool value) {
    _ParentAsPivot = value;
}

bool SSceneComTransform::getParentAsPivot() const {
    return _ParentAsPivot && _ParentTransform;
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
            /* Parent was set before this. */
            _Position = getPositionAbs();
            _Scale = getScaleAbs();
            _Angle = getAngleAbs();
            _ParentAsPivot = false;

            _ParentTransform = nullptr;
            _IsChanged = true;
        }
        return;
    }

    if (_ParentTransform == _Owner->getParent()->getTransform()) {
        /* Same parent is already set */
        return;
    }

    _ParentTransform = _Owner->getParent()->getTransform();
    _IsChanged = true;
}
