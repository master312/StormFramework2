#include "SSceneComPlane.h"
#include "SSceneComTransform.h"
#include "../StormSceneObject.h"
#include "../../core/StormCommon.h"
#include "../../core/utils/math/ScalarMath.h"
#include "../../core/utils/math/TrigonometryMath.h"

SSceneComPlane::SSceneComPlane(StormSceneObject* owner) : SSceneComponent(owner) {
    _Type = S_SCENE_OBJECT_COM_PLANE;
    
    _Transform = nullptr;

    _Size.set(0.0f, 0.0f);
    _SizeTransformed.set(0.0f, 0.0f);

    _Vertices[0].uv.set(0.0f, 0.0f);
    _Vertices[1].uv.set(1.0f, 0.0f);
    _Vertices[2].uv.set(1.0f, 1.0f);
    _Vertices[3].uv.set(0.0f, 1.0f);

    _RenderDebug = false;
    _InheritScale = true;
    _InheritRotation = true;
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

void SSceneComPlane::initialize() {
    _Transform = _Owner->getTransform();
    if (!_Transform) {
        LOG(ERROR) << "Plane component could not get transform from object " << _Owner->getName();
        return;
    }

    observeTransformChanged(nullptr);

    S_OBSERVER_ADD(_Owner, this, S_OBSERVER_EVENT_TRANSFORM_UPDATED, SSceneComPlane::observeTransformChanged);
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

void SSceneComPlane::setInheritRotation(bool value) {
    _InheritRotation = value; 
}

bool SSceneComPlane::getInheritRotation() const {
    return _InheritRotation;
}

void SSceneComPlane::setInheritScale(bool value) {
    _InheritScale = value;
}

bool SSceneComPlane::getInheritScale() const {
    return _InheritScale;
}

StormVertex* SSceneComPlane::getVertices() {
    return _Vertices;
}

bool SSceneComPlane::isRenderDebug() {
    return _RenderDebug;
}

void SSceneComPlane::setRenderDebug(bool shouldRender) {
    _RenderDebug = shouldRender;
}

void SSceneComPlane::observeTransformChanged(void* data) {
    if (!_Transform) {
        /* Transform component dose not exists, and plane can not exist without transform */
        LOG(ERROR) << "Plane component exists on scene object without transform component";
        return;
    }
    
    _SizeTransformed = _Size.mult(_Transform->getScale());

    StormSceneObject* parent = _Owner->getParent();
    if (parent) {
        if (_InheritScale) {
            _SizeTransformed = _Size.mult(_Transform->getScale().mult( 
                                          parent->getTransform()->getScale()));
        }
        transformRotationWithParent();
    } else {
        transformRotation(_Transform->getPositionAbs());
    }
}

void SSceneComPlane::transformRotation(const Vector2& centerPosition) {
    calculateVertices(centerPosition);

    float angle = _Transform->getAngle();
    if (StormScalarMath::equivalent(angle, 0.0f)) {
        /* Do not rotate if angle is zero */
        return;
    }
    float sin = StormScalarMath::sin((angle * MATH_PI) / 180.0f);
    float cos = StormScalarMath::cos((angle * MATH_PI) / 180.0f);
    for (int i = 0; i < 4; i++) {
        Vector2 tmpPoint = _Vertices[i].position;
        tmpPoint.x = centerPosition.x - tmpPoint.x;
        tmpPoint.y -= centerPosition.y;
        _Vertices[i].position.x = (tmpPoint.x * cos - tmpPoint.y * sin) + centerPosition.x;
        _Vertices[i].position.y = (tmpPoint.y * cos + tmpPoint.x * sin) + centerPosition.y;
    }
}

void SSceneComPlane::transformRotationWithParent() {
    if (!_InheritRotation) {
        transformRotation(_Transform->getPositionAbs());
        return;
    }

    transformRotation(_Transform->getPosition());
    
    SSceneComTransform* parentTransform = _Owner->getParent()->getTransform();
    float angle = parentTransform->getAngleAbs();
    Vector2 pivotPos = parentTransform->getPositionAbs();
    float sin = StormScalarMath::sin((angle * MATH_PI) / 180.0f);
    float cos = StormScalarMath::cos((angle * MATH_PI) / 180.0f);
    for (int i = 0; i < 4; i++) {
        Vector2 tmpPoint = _Vertices[i].position;
        _Vertices[i].position.x = (tmpPoint.x * cos - tmpPoint.y * sin) + pivotPos.x;
        _Vertices[i].position.y = (tmpPoint.y * cos + tmpPoint.x * sin) + pivotPos.y;
    }
}

void SSceneComPlane::calculateVertices(const Vector2& center) {
    Vector2 size2 = _SizeTransformed / 2;
    _Vertices[0].position = center - size2;
    _Vertices[1].position = center;
    _Vertices[1].position.x += size2.x;
    _Vertices[1].position.y -= size2.y;
    _Vertices[2].position = center + size2;
    _Vertices[3].position = center;
    _Vertices[3].position.x -= size2.x;
    _Vertices[3].position.y += size2.y;
}