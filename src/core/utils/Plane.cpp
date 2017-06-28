#include "Plane.h"
#include "math/ScalarMath.h"
#include "math/TrigonometryMath.h"

Plane::Plane() {
    /* No need to set Vector2 variables. They will be set to 0.0f by default */
    _Angle = 0.0f;
    _Scale.set(1.0f, 1.0f);
}

Plane::~Plane() {
}

void Plane::transform(Plane* parent /* = nullptr */) {
    _PivotPositionTransformed = _PivotPosition;
    if (parent && !StormScalarMath::equivalent(parent->getAngle(), 0)) {
        /* Rotate pivot around parent */
        StormTrigonometryMath::rotatePointAround(_PivotPositionTransformed, 
                                                parent->getPositionTransformed(), parent->getAngle());
    }
    transformScale(parent);
    
    Vector2 size2 = _SizeTransformed / 2;
    _Vertices[0] = _PlanePivotOffset - size2;
    _Vertices[1] = _PlanePivotOffset;
    _Vertices[1].x += size2.x;
    _Vertices[1].y -= size2.y;
    _Vertices[2] = _PlanePivotOffset + size2;
    _Vertices[3] = _PlanePivotOffset;
    _Vertices[3].x -= size2.x;
    _Vertices[3].y += size2.y;
    
    /* Rotate all points around pivot */
    /* TODO: Optimize. Dont calculate sin and cos in ::rotatePointAround() for every point */
    for (int i = 0; i < 4; i++) {
        StormTrigonometryMath::rotatePointAround(_Vertices[i], _PivotPositionTransformed, getAngle());
    }
}

void Plane::setPosition(const Vector2& position) {
    _PivotPosition = position;
}

Vector2 Plane::getPosition() const {
    return _PivotPosition;
}

Vector2 Plane::getPositionTransformed() const {
    return _PivotPositionTransformed;
}

void Plane::setPlaneOffset(const Vector2& offset)  {
    _PlanePivotOffset = offset;
}

Vector2 Plane::getPlaneOffset() const {
    return _PlanePivotOffset;
}

Vector2 Plane::getPlaneOffsetTransformed() const {
    return _PivotPositionTransformed;
}

void Plane::setAngle(float angle) {
    _Angle = angle;
}

void Plane::addAngle(float angle) {
    _Angle += angle;
    if (_Angle > 360.0f) {
        _Angle -= 360.0f;
    } else if (_Angle < -360.0f) {
        _Angle += 360.0f;
    }
}

float Plane::getAngle() {
    return _Angle;
}

void Plane::setSize(const Vector2& size) {
    _Size = size;
}

Vector2 Plane::getSize() const {
    return _Size;
}

Vector2 Plane::getSizeTransformed() const {
    return _SizeTransformed;
}

void Plane::setScale(const Vector2& scale) {
    _Scale = scale;
}

Vector2 Plane::getScale() const {
    return _Scale;
}

Vector2* Plane::getVertices() {
    return _Vertices;
}

Vector2* Plane::getVertex(int ver) {
    return& _Vertices[ver];
}

void Plane::transformScale(Plane* parent) {
    _SizeTransformed = _Scale;
    if (parent) {
        _SizeTransformed += parent->getScale();
        _SizeTransformed.x -= 1.0f;
        _SizeTransformed.y -= 1.0f;
    }
    _SizeTransformed.x *= _Size.x;
    _SizeTransformed.y *= _Size.y;
}