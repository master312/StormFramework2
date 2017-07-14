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
    SSceneComponent* component = _Owner->getComponent(S_SCENE_OBJECT_COM_TRANSFORM);
    _Transform = dynamic_cast<SSceneComTransform*>(component);
    if (!_Transform) {
        LOG(ERROR) << "Plane component could not get transform from object " << _Owner->getName();
        return;
    }

    onTransformChanged();
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
    return _Vertices;
}

bool SSceneComPlane::isRenderDebug() {
    return _RenderDebug;
}

void SSceneComPlane::setRenderDebug(bool shouldRender) {
    _RenderDebug = shouldRender;
}

void SSceneComPlane::onTransformChanged() {
    if (!_Transform) {
        /* Transform component dose not exists, so plane can not exist too */
        LOG(ERROR) << "Plane component exists on scene object without transform component";
        return;
    }
    Vector2* ownerPosition = _Transform->getPositionAbsPtr();

    /* Transform scale */
    _SizeTransformed = _Size;
    _SizeTransformed.x *= _Transform->getScale().x;   // TODO: Micro optimize
    _SizeTransformed.y *= _Transform->getScale().y;

    /* Generate vertex positions */
    Vector2 halfSize = _SizeTransformed / 2;
    _Vertices[0].position = *ownerPosition - halfSize;
    _Vertices[1].position = *ownerPosition;
    _Vertices[1].position.x += halfSize.x;
    _Vertices[1].position.y -= halfSize.y;
    _Vertices[2].position = *ownerPosition + halfSize;
    _Vertices[3].position = *ownerPosition;
    _Vertices[3].position.x -= halfSize.x;
    _Vertices[3].position.y += halfSize.y;

    /* Rotate all vertices if angle is changed */
    if (!StormScalarMath::equivalent(_Transform->getAngle(), 0.0f)) {
        float sin = StormScalarMath::sin((_Transform->getAngle() * MATH_PI) / 180.0f);
        float cos = StormScalarMath::cos((_Transform->getAngle() * MATH_PI) / 180.0f);
        for (int i = 0; i < 4; i++) {
            Vector2 tmpPoint = _Vertices[i].position;
            _Vertices[i].position.x = (tmpPoint.x * cos - tmpPoint.y * sin);
            _Vertices[i].position.y = (tmpPoint.y * cos + tmpPoint.x * sin);
        }
    }
}