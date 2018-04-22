#include "SComTransform.h"
#include "scene/SSceneObject.h"
#include "utils/math/ScalarMath.h"

SComTransform::SComTransform(SSceneObject* owner) : SSceneComponent(owner) {
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

SComTransform::~SComTransform() {
    _ParentTransform = nullptr;
}

void SComTransform::serializeXml(pugi::xml_node& node) {
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

int SComTransform::deserializeXml(pugi::xml_node& node) {
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

int SComTransform::initialize(SSceneComponentSystem* system) {
    SSceneComponent::initialize(system);

    pullParentTransform();
    transform();
    
    _Owner->registerObserver<SComTransform>(
            S_EVENT_PARENT_CHANGED, &SComTransform::observeParentChanged, this);
    _Owner->registerObserver<SComTransform>(
            S_EVENT_PARENT_TRANSFORM_UPDATED, &SComTransform::observeParentTransformChanged, this);

    return 1;
}

void SComTransform::transform() {
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
    _Owner->notifyObservers(S_EVENT_TRANSFORM_UPDATED);
    
    for (auto* child : _Owner->getChildren()) {
        /* Notify all children that parent's transformation has been updated */
        child->notifyObservers(S_EVENT_PARENT_TRANSFORM_UPDATED);
    }
    
    _IsChanged = false;
}

Vector2 SComTransform::getPosition() const {
    return _Position;
}

void SComTransform::setPosition(const Vector2 position) {
    setPosition(position.x, position.y);
}

void SComTransform::setPosition(float x, float y) {
    if (StormScalarMath::equivalent(_Position.x, x) &&
        StormScalarMath::equivalent(_Position.y, y)) {
            return;
    }
    _Position.x = x;
    _Position.y = y;
    _IsChanged = true;
}

void SComTransform::setPositionAbs(const Vector2 position) {
    if (!_ParentTransform) {
        setPosition(position);
        return;
    }
    
    setPosition(position - _ParentTransform->getPositionAbs());
}

void SComTransform::setPositionAbs(float x, float y) {
    if (!_ParentTransform) {
        setPosition(x, y);
        return;
    }
    setPosition(x - _ParentTransform->getAbsX(), 
                y - _ParentTransform->getAbsY());
}

void SComTransform::setX(float x) {
    setPosition(x, _Position.y);
}

void SComTransform::setY(float y) {
    setPosition(_Position.x, y);
}

float SComTransform::getX() const {
    return _Position.x;
}

float SComTransform::getY() const {
    return _Position.y;
}

Vector2 SComTransform::getPositionAbs() const {
    return _PositionAbs;
}

float SComTransform::getAbsX() const {
    return _PositionAbs.x;
}

float SComTransform::getAbsY() const {
    return _PositionAbs.y;
}

Vector2 SComTransform::getScale() const {
    return _Scale;
}

Vector2 SComTransform::getScaleAbs() const {
    return _ScaleAbs;
}

void SComTransform::setScale(Vector2 scale) {
    if (_Scale == scale) {
        return;
    }
    _Scale = scale;
    _IsChanged = true;
}

int SComTransform::getZ() const {
    return _ZPriority;
}

int SComTransform::getAbsZ() const {
    if (!_ParentTransform) {
        return _ZPriority;
    }
    return _ZPriority + _ParentTransform->getZ();
}

void SComTransform::setZ(int value) {
    _ZPriority = value;
}

float SComTransform::getAngle() const {
    return _Angle;
}

float SComTransform::getAngleAbs() const {
    if (_ParentTransform) {
        return StormScalarMath::clampAngle(_ParentTransform->getAngleAbs() + _Angle);
    }
    return _Angle;
}

void SComTransform::setAngle(float angle) {
    angle = StormScalarMath::clampAngle(angle);
    if (StormScalarMath::equivalent(angle, _Angle)) {
        return;
    }
    _Angle = angle;
    _IsChanged = true;
}

void SComTransform::setInheritScale(bool value) {
    if (_InheritScale == value) {
        return;
    }
    _InheritScale = value;
    _IsChanged = true;
}

bool SComTransform::getInheritScale() const {
    return _InheritScale;
}

void SComTransform::setParentAsPivot(bool value) {
    _ParentAsPivot = value;
}

bool SComTransform::getParentAsPivot() const {
    return _ParentAsPivot && _ParentTransform;
}

bool SComTransform::isChanged() {
    return _IsChanged;
}

void SComTransform::observeParentChanged(void* data) {
    pullParentTransform();
}

void SComTransform::observeParentTransformChanged(void* data) {
    _IsChanged = true;
}

void SComTransform::onLuaBinded(bool hasScript) {
    /* For easier access to position in script */
    sol::table scriptTable = getOwnerLuaHandle();
    if (hasScript) {
        scriptTable = scriptTable["script"];
    }
    scriptTable.set_function("getPosAbs", &SComTransform::getPositionAbs, this);
}

void SComTransform::pullParentTransform() {
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
