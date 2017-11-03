#include "Geometry.h"

Geometry::Geometry() {
    _HasChanged = false;
    _Type = GEOMETRY_TYPE_NONE;
}

Geometry::~Geometry() {
}

GeometryType Geometry::getType() {
    return _Type;
}

void Geometry::setPosition(const Vector2& position) {
    _Position = position;
    _HasChanged = true;
}

void Geometry::setPositionX(float x) {
    _Position.x = x;
    _HasChanged = true;
}

void Geometry::setPositionY(float y) {
    _Position.y = y;
    _HasChanged = true;
}

Vector2 Geometry::getPosition() const {
    return _Position;
}

float Geometry::getPositionX() {
    return _Position.x;
}

float Geometry::getPositionY() {
    return _Position.y;
}
