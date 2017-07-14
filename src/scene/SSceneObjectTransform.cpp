#include "SSceneObjectTransform.h"

SSceneObjectTransform::SSceneObjectTransform(StormSceneObject* owner) {
    _OwnerObject = owner;
    _Position.set(0.0f, 0.0f);
    _PositionAbs.set(0.0f, 0.0f);
    _Scale.set(1.0f, 1.0f);
    _Angle = 0.0f;
}

SSceneObjectTransform::~SSceneObjectTransform() {
}

void SSceneObjectTransform::transform(SSceneObjectTransform* parent) {
    if (!parent) {
        _PositionAbs.set(_Position);
        return;
    }
    
}

Vector2 SSceneObjectTransform::getPosition() {
    return _Position;
}

Vector2* SSceneObjectTransform::getPositionPtr() {
    return &_Position;
}

void SSceneObjectTransform::setPosition(Vector2 position) {
    _Position = position;
}

Vector2 SSceneObjectTransform::getPositionAbs() {
    return _PositionAbs;
}

Vector2* SSceneObjectTransform::getPositionAbsPtr() {
    return &_PositionAbs;
}

Vector2 SSceneObjectTransform::getScale() {
    return _Scale;
}

Vector2* SSceneObjectTransform::getScalePtr() {
    return &_Scale;
}

void SSceneObjectTransform::setScale(Vector2 scale) {
    _Scale = scale;
}

float SSceneObjectTransform::getAngle() {
    return _Angle;
}

float* SSceneObjectTransform::getAnglePtr() {
    return &_Angle;
}

void SSceneObjectTransform::setAngle(float angle) {
    _Angle = angle;
}