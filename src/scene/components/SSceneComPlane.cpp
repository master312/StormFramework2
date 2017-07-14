#include "SSceneComPlane.h"
#include "../StormSceneObject.h"
#include "../../core/utils/math/ScalarMath.h"
#include "../../core/utils/math/TrigonometryMath.h"

SSceneComPlane::SSceneComPlane(StormSceneObject* owner) : SSceneComponent(owner) {
    _Type = S_SCENE_OBJECT_COM_PLANE;
    
    _Size.set(0.0f, 0.0f);
    _SizeTransformed.set(0.0f, 0.0f);

    _Vertices[0].uv.set(0.0f, 0.0f);
    _Vertices[1].uv.set(1.0f, 0.0f);
    _Vertices[2].uv.set(1.0f, 1.0f);
    _Vertices[3].uv.set(0.0f, 1.0f);
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

    /* TODO: remove this. This is only temporary */
    onTransformChanged();
    return 1;
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
    SSceneObjectTransform& transform = _Owner->getTransform();
    Vector2* ownerPosition = transform.getPositionAbsPtr();

    /* Transform scale */
    _SizeTransformed = _Size;
    _SizeTransformed.x *= transform.getScale().x;   // TODO: Micro optimize
    _SizeTransformed.y *= transform.getScale().y;

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
    if (!StormScalarMath::equivalent(transform.getAngle(), 0.0f)) {
        float sin = StormScalarMath::sin((transform.getAngle() * MATH_PI) / 180.0f);
        float cos = StormScalarMath::cos((transform.getAngle() * MATH_PI) / 180.0f);
        for (int i = 0; i < 4; i++) {
            Vector2 tmpPoint = _Vertices[i].position;
            _Vertices[i].position.x = (tmpPoint.x * cos - tmpPoint.y * sin) + ownerPosition->x;
            _Vertices[i].position.y = (tmpPoint.y * cos + tmpPoint.x * sin) + ownerPosition->y;
        }
    }
}