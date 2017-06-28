#include "SSceneComPlane.h"
#include "../../StormEngineEditing.h"
#include "../../core/utils/math/ScalarMath.h"
#include "../../core/utils/math/TrigonometryMath.h"

SSceneComPlane::SSceneComPlane(StormSceneObject* owner) : SSceneComponent(owner) {
    _Type = S_SCENE_OBJECT_COM_PLANE;
    
    _Angle = 0.0f;
    _Scale.set(1.0f, 1.0f);

    _Vertices[0].uv.set(0.0f, 0.0f);
    _Vertices[1].uv.set(1.0f, 0.0f);
    _Vertices[2].uv.set(1.0f, 1.0f);
    _Vertices[3].uv.set(0.0f, 1.0f);
}

SSceneComPlane::~SSceneComPlane() {
}

void SSceneComPlane::serializeXml(pugi::xml_node& node) {
    SSceneComponent::serializeXml(node);

    node.append_attribute("pivot_x").set_value(_PivotPosition.x);
    node.append_attribute("pivot_y").set_value(_PivotPosition.y);
    node.append_attribute("center_x").set_value(_PlaneCenterPosition.x);
    node.append_attribute("center_y").set_value(_PlaneCenterPosition.y);
    node.append_attribute("size_x").set_value(_Size.x);
    node.append_attribute("size_y").set_value(_Size.y);
    node.append_attribute("scale_x").set_value(_Scale.x);
    node.append_attribute("scale_y").set_value(_Scale.y);
    node.append_attribute("angle").set_value(_Angle);
}

int SSceneComPlane::deserializeXml(pugi::xml_node& node) {
    SSceneComponent::deserializeXml(node);

    _PivotPosition.x = node.attribute("pivot_x").as_float(0.0f);
    _PivotPosition.y = node.attribute("pivot_y").as_float(0.0f);
    _PlaneCenterPosition.x = node.attribute("center_x").as_float(0.0f);
    _PlaneCenterPosition.y = node.attribute("center_y").as_float(0.0f);
    _Size.x = node.attribute("size_x").as_float(0.0f);
    _Size.y = node.attribute("size_y").as_float(0.0f);
    _Scale.x = node.attribute("scale_x").as_float(1.0f);
    _Scale.y = node.attribute("scale_y").as_float(1.0f);
    _Angle = node.attribute("angle").as_float();

    /* TODO: Probably dose not need to transform here. 
     * Fix this later when we have scene initialization system. */
    transform();

    return 1;
}

void SSceneComPlane::transform(Plane* parent /* = nullptr */) {
    _PivotPositionTransformed = _PivotPosition;
    if (parent && !StormScalarMath::equivalent(parent->getAngle(), 0)) {
        /* Rotate pivot around parent */
        StormTrigonometryMath::rotatePointAround(_PivotPositionTransformed, 
                                                parent->getPositionTransformed(), parent->getAngle());
    }
    transformScale(parent);
    
    Vector2 size2 = _SizeTransformed / 2;
    _Vertices[0].position = _PlaneCenterPosition - size2;
    _Vertices[1].position = _PlaneCenterPosition;
    _Vertices[1].position.x += size2.x;
    _Vertices[1].position.y -= size2.y;
    _Vertices[2].position = _PlaneCenterPosition + size2;
    _Vertices[3].position = _PlaneCenterPosition;
    _Vertices[3].position.x -= size2.x;
    _Vertices[3].position.y += size2.y;
    
    /* Rotate all points around pivot */
    float sin = StormScalarMath::sin(_Angle);
    float cos = StormScalarMath::cos(_Angle);
    for (int i = 0; i < 4; i++) {
        Vector2 tmpPoint = _Vertices[i].position;
        _Vertices[i].position.x = (tmpPoint.x * cos - tmpPoint.y * sin) + _PivotPositionTransformed.x;
        _Vertices[i].position.y = (tmpPoint.y * cos + tmpPoint.x * sin) + _PivotPositionTransformed.y;
    }
}

void SSceneComPlane::setPosition(const Vector2& position) {
    _PivotPosition = position;
}

Vector2 SSceneComPlane::getPosition() const {
    return _PivotPosition;
}

Vector2 SSceneComPlane::getPositionTransformed() const {
    return _PivotPositionTransformed;
}

void SSceneComPlane::setCenterPosition(const Vector2& position) {
    _PlaneCenterPosition = position;
}

Vector2 SSceneComPlane::getCenterPosition() const {
    return _PlaneCenterPosition;
}

Vector2 SSceneComPlane::getCenterPositionTransformed() const {
    return _PlaneCenterTransformed;
}

void SSceneComPlane::setAngle(float angle) {
    _Angle = angle;
    StormScalarMath::clampAngle(&_Angle);
}

void SSceneComPlane::addAngle(float angle) {
    StormScalarMath::clampAngle(&angle);
    _Angle += angle;
    StormScalarMath::clampAngle(&_Angle);
}

float SSceneComPlane::getAngle() {
    return _Angle;
}

void SSceneComPlane::setSize(const Vector2& size) {
    _Size = size;
}

Vector2 SSceneComPlane::getSize() const {
    return _Size;
}

Vector2 SSceneComPlane::getSizeTransformed() const {
    return _SizeTransformed;
}

void SSceneComPlane::setScale(const Vector2& scale) {
    _Scale = scale;
}

Vector2 SSceneComPlane::getScale() const {
    return _Scale;
}

StormVertex* SSceneComPlane::getVertices() {
    return _Vertices;
}

void SSceneComPlane::transformScale(Plane* parent) {
    _SizeTransformed = _Scale;
    if (parent) {
        _SizeTransformed += parent->getScale();
        _SizeTransformed.x -= 1.0f;
        _SizeTransformed.y -= 1.0f;
    }
    _SizeTransformed.x *= _Size.x;
    _SizeTransformed.y *= _Size.y;
}

#ifdef _EDITING_SUPPORT

void SSceneComPlane::renderEditingGui() {
    bool shoudTransform = false;
    shoudTransform |= ImGui::DragFloat("Position X", &_PivotPosition.x, 5.0f);
    shoudTransform |= ImGui::DragFloat("Position Y", &_PivotPosition.y, 5.0f);
    shoudTransform |= ImGui::DragFloat("Pivot X", &_PlaneCenterPosition.x, 5.0f);
    shoudTransform |= ImGui::DragFloat("Pivot Y", &_PlaneCenterPosition.y, 5.0f);
    shoudTransform |= ImGui::DragFloat("Scale X", &_Scale.x, 0.05f);
    shoudTransform |= ImGui::DragFloat("Scale Y", &_Scale.y, 0.05f);
    shoudTransform |= ImGui::SliderAngle("Angle", &_Angle);
    if (shoudTransform) {
        transform();
    }
}

#endif