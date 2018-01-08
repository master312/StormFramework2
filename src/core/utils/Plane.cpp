#include "Plane.h"
#include "math/ScalarMath.h"
#include "math/TrigonometryMath.h"

Plane::Plane() {
    _Angle = 0.0f;
    _PivotAngle = 0.0f;

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
    
    calculatePoints(_PivotPosition, true);

    float sin = StormScalarMath::sin((_Angle * MATH_PI) / 180.0f);
    float cos = StormScalarMath::cos((_Angle * MATH_PI) / 180.0f);
    float pivotSin = StormScalarMath::sin((_PivotAngle * MATH_PI) / 180.0f);
    float pivotCos = StormScalarMath::cos((_PivotAngle * MATH_PI) / 180.0f);

    for (int i = 0; i < 4; i++) {
        Vector2 tmpPoint = _Points[i];
        tmpPoint.x = _PivotPosition.x - tmpPoint.x;
        tmpPoint.y -= _PivotPosition.y;

        Vector2 tmpPoint2;
        tmpPoint2.x = (tmpPoint.x * cos - tmpPoint.y * sin);
        tmpPoint2.y = (tmpPoint.y * cos + tmpPoint.x * sin);

        _Points[i].x = (tmpPoint2.x * pivotCos - tmpPoint2.y * pivotSin) + _Position.x;
        _Points[i].y = (tmpPoint2.y * pivotCos + tmpPoint2.x * pivotSin) + _Position.y;
    }

    _PivotPosition *= -1;
}

void Plane::transformRotationAroundCenter() {
    if (StormScalarMath::equivalent(_Angle, 0.0f)) {
        /* Plane is not rotated. Dose not need to calculate anything else */
        calculatePoints(_Position);
        return;   
    }
    calculatePoints(_Position, true);
    
    float sin = StormScalarMath::sin((_Angle * MATH_PI) / 180.0f);
    float cos = StormScalarMath::cos((_Angle * MATH_PI) / 180.0f);
    for (int i = 0; i < 4; i++) {
        Vector2 tmpPoint = _Points[i];
        tmpPoint.x = _Position.x - tmpPoint.x;
        tmpPoint.y -= _Position.y;
        _Points[i].x = (tmpPoint.x * cos - tmpPoint.y * sin) + _Position.x;
        _Points[i].y = (tmpPoint.y * cos + tmpPoint.x * sin) + _Position.y;
    }
}

bool Plane::containsPoint(const Vector2& point) {
    Vector2& p1 = _Points[0];
    //Vector2& p2 = _Points[1];
    Vector2& p3 = _Points[2];
    Vector2& p4 = _Points[3];

    Vector2 p1_p4 = p1 - p4;
    Vector2 p3_p4 = p3 - p4;
   // TWO_P_C=2P-C, C=Center of rectangle
    Vector2 pwo_p_c = (point * 2.0f) - p1 - p3;
   
    return (p3_p4.dot(pwo_p_c - p3_p4) <= 0 && p3_p4.dot(pwo_p_c + p3_p4) >= 0) &&
           (p1_p4.dot(pwo_p_c - p1_p4) <= 0 && p1_p4.dot(pwo_p_c + p1_p4) >= 0);
}

void Plane::calculatePoints(const Vector2& center, bool flipX /* = false */) {
    Vector2 size2 = _Size / 2;
    if (flipX) {
        _Points[1] = center - size2;
        _Points[0] = center;
        _Points[0].x += size2.x;
        _Points[0].y -= size2.y;
        _Points[3] = center + size2;
        _Points[2] = center;
        _Points[2].x -= size2.x;
        _Points[2].y += size2.y;
        return;
    }
    _Points[0] = center - size2;
    _Points[1] = center;
    _Points[1].x += size2.x;
    _Points[1].y -= size2.y;
    _Points[2] = center + size2;
    _Points[3] = center;
    _Points[3].x -= size2.x;
    _Points[3].y += size2.y;
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

Vector2* Plane::getPoints() {
    return _Points;
}
