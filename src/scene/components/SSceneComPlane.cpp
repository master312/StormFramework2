#include "SSceneComPlane.h"
#include "SSceneComTransform.h"
#include "../StormSceneObject.h"
#include "../../core/StormCommon.h"
#include "../../core/utils/math/ScalarMath.h"
#include "../../core/utils/math/TrigonometryMath.h"

SSceneComPlane::SSceneComPlane(StormSceneObject* owner) : SSceneComponent(owner) {
    _Type = S_SCENE_OBJECT_COM_PLANE;

    _Size.set(0.0f, 0.0f);
    _SizeTransformed.set(0.0f, 0.0f);

    _RenderDebug = false;
}

SSceneComPlane::~SSceneComPlane() {
}

void SSceneComPlane::serializeXml(pugi::xml_node& node) {
    SSceneComponent::serializeXml(node);

    node.append_attribute("size_x").set_value(_Size.x);
    node.append_attribute("size_y").set_value(_Size.y);
}

int SSceneComPlane::deserializeXml(pugi::xml_node& node) {
    SSceneComponent::deserializeXml(node);

    _Size.x = node.attribute("size_x").as_float(0.0f);
    _Size.y = node.attribute("size_y").as_float(0.0f);

    return 1;
}

int SSceneComPlane::initialize(SSceneComponentSystem* system) {
    if (!_Owner->getTransform()) {
        LOG(ERROR) << "Plane component could not get transform from object " << _Owner->getName();
        return -1;
    }
    SSceneComponent::initialize(system);

    observeTransformChanged(nullptr);

    S_OBSERVER_ADD(_Owner, this, S_OBSERVER_EVENT_TRANSFORM_UPDATED, SSceneComPlane::observeTransformChanged);
    return 1;
}

void SSceneComPlane::observeTransformChanged(void* data) {
    SSceneComTransform* transform = _Owner->getTransform();

    if (!transform) {
        /* Transform component dose not exists, and plane can not exist without transform */
        LOG(ERROR) << "Plane component exists on scene object without transform component";
        return;
    }
    
    _SizeTransformed = _Size.mult(transform->getScaleAbs());
    
    _Plane.setSize(_SizeTransformed);
    _Plane.setPosition(transform->getPositionAbs());
    _Plane.setAngle(transform->getAngle());

    if (transform->getParentAsPivot()) {
        _Plane.setPivot(transform->getPosition() * -1);
        _Plane.setPivotAngle(transform->getAngleAbs());
    }

    _Plane.transform();
}

void SSceneComPlane::setSize(const Vector2 size) {
    _Size = size;
}

Vector2 SSceneComPlane::getSize() const {
    return _Size;
}

Vector2 SSceneComPlane::getSizeTransformed() const {
    return _SizeTransformed;
}

StormVertex* SSceneComPlane::getVertices() {
    return _Plane.getVertices();
}

bool SSceneComPlane::isRenderDebug() {
    return _RenderDebug;
}

void SSceneComPlane::setRenderDebug(bool shouldRender) {
    _RenderDebug = shouldRender;
}

bool SSceneComPlane::containsPoint(const Vector2& point) {
    return _Plane.containsPoint(point);
}