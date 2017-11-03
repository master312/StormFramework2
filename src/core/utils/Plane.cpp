#include "Plane.h"
#include "math/ScalarMath.h"
#include "math/TrigonometryMath.h"

Plane::Plane() {
    _Angle = 0.0f;
    _PivotAngle = 0.0f;

    /* Set default UV coordinates */
    _Vertices[0].uv.set(0.0f, 0.0f);
    _Vertices[1].uv.set(1.0f, 0.0f);
    _Vertices[2].uv.set(1.0f, 1.0f);
    _Vertices[3].uv.set(0.0f, 1.0f);
    _Type = GEOMETRY_TYPE_PLANE;
}

Plane::~Plane() {
}

void Plane::transform() {
    if (!_HasChanged) {
        return;
    }
    _HasChanged = false;

    if ((_PivotPosition.x != 0.0f || _PivotPosition.y != 0.0f) && !StormScalarMath::equivalent(_PivotAngle, 0.0f)) {
        /* Rotate around pivot */
        transformRotationAroundPivot();
    } else {
        /* There is no pivot. So only rotate around center*/
        transformRotationAroundCenter();
    }

}

void Plane::transformRotationAroundPivot() {
    _PivotPosition *= -1;
    
    calculateVertices(_PivotPosition);

    float sin = StormScalarMath::sin((_Angle * MATH_PI) / 180.0f);
    float cos = StormScalarMath::cos((_Angle * MATH_PI) / 180.0f);
    float pivotSin = StormScalarMath::sin((_PivotAngle * MATH_PI) / 180.0f);
    float pivotCos = StormScalarMath::cos((_PivotAngle * MATH_PI) / 180.0f);
    for (int i = 0; i < 4; i++) {
        Vector2 tmpPoint = _Vertices[i].position;
        tmpPoint.x = _PivotPosition.x - tmpPoint.x;
        tmpPoint.y -= _PivotPosition.y;

        Vector2 tmpPoint2;
        tmpPoint2.x = (tmpPoint.x * cos - tmpPoint.y * sin) + _PivotPosition.x;
        tmpPoint2.y = (tmpPoint.y * cos + tmpPoint.x * sin) + _PivotPosition.y;

        _Vertices[i].position.x = (tmpPoint2.x * pivotCos - tmpPoint2.y * pivotSin) + _Position.x;
        _Vertices[i].position.y = (tmpPoint2.y * pivotCos + tmpPoint2.x * pivotSin) + _Position.y;
    }

    _PivotPosition *= -1;
}

void Plane::transformRotationAroundCenter() {
    calculateVertices(_Position);
    
    if (StormScalarMath::equivalent(_Angle, 0.0f)) {
        /* Plane is not rotated. Dose not need to calculate anything else */
        return;   
    }
    
    float sin = StormScalarMath::sin((_Angle * MATH_PI) / 180.0f);
    float cos = StormScalarMath::cos((_Angle * MATH_PI) / 180.0f);
    for (int i = 0; i < 4; i++) {
        Vector2 tmpPoint = _Vertices[i].position;
        tmpPoint.x = _Position.x - tmpPoint.x;
        tmpPoint.y -= _Position.y;
        _Vertices[i].position.x = (tmpPoint.x * cos - tmpPoint.y * sin) + _Position.x;
        _Vertices[i].position.y = (tmpPoint.y * cos + tmpPoint.x * sin) + _Position.y;
    }
}

bool Plane::containsPoint(const Vector2& point) {
    Vector2& p1 = _Vertices[0].position;
    Vector2& p2 = _Vertices[1].position;
    Vector2& p3 = _Vertices[2].position;
    Vector2& p4 = _Vertices[3].position;

    Vector2 p1_p4 = p1 - p4;
    Vector2 p3_p4 = p3 - p4;
   // TWO_P_C=2P-C, C=Center of rectangle
    Vector2 pwo_p_c = (point * 2.0f) - p1 - p3;
   
    return (p3_p4.dot(pwo_p_c - p3_p4) <= 0 && p3_p4.dot(pwo_p_c + p3_p4) >= 0) &&
           (p1_p4.dot(pwo_p_c - p1_p4) <= 0 && p1_p4.dot(pwo_p_c + p1_p4) >= 0);
}

void Plane::calculateVertices(const Vector2& center) {
    Vector2 size2 = _Size / 2;
    _Vertices[0].position = center - size2;
    _Vertices[1].position = center;
    _Vertices[1].position.x += size2.x;
    _Vertices[1].position.y -= size2.y;
    _Vertices[2].position = center + size2;
    _Vertices[3].position = center;
    _Vertices[3].position.x -= size2.x;
    _Vertices[3].position.y += size2.y;
}

void Plane::setPivot(const Vector2& position) {
    _PivotPosition = position;
    _HasChanged = true;
}

void Plane::setPivotX(float x) {
    _PivotPosition.x = x;
    _HasChanged = true;
}

void Plane::setPivotY(float y) {
    _PivotPosition.y = y;
    _HasChanged = true;
}

Vector2 Plane::getPivot() const {
    return _PivotPosition;
}

float Plane::getPivotX() {
    return _PivotPosition.x;
}

float Plane::getPivotY() {
    return _PivotPosition.y;
}

void Plane::setSize(const Vector2& size) {
    _Size = size;
    _HasChanged = true;
}

void Plane::setSizeX(float x) {
    _Size.x = x;
    _HasChanged = true;
}

void Plane::setSizeY(float y) {
    _Size.y = y;
    _HasChanged = true;
}

Vector2 Plane::getSize() const {
    return _Size;
}

float Plane::getSizeX() {
    return _Size.x;
}

float Plane::getSizeY() {
    return _Size.y;
}

void Plane::setAngle(float angle) {
    _Angle = angle;
    _HasChanged = true;
}

float Plane::getAngle() {
    return _Angle;
}

void Plane::setPivotAngle(float angle) {
    _PivotAngle = angle;
    _HasChanged = true;
}

float Plane::getPivotAngle() {
    return _PivotAngle;
}

StormVertex* Plane::getVertices() {
    return _Vertices;
}
