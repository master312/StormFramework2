#include "SSceneSystemTransform.h"
#include "SSceneComTransform.h"
#include "scene/SScene.h"
#include "scene/components/luaScript/SSceneSystemLuaScript.h"

SSceneSystemTransform::SSceneSystemTransform(SScene* scene) : SSceneComponentSystem(scene) {
    _Type = S_SCENE_OBJECT_COM_TRANSFORM;
}

SSceneSystemTransform::~SSceneSystemTransform() {
}

void SSceneSystemTransform::tick(float deltaTime) {
    for (SSceneComTransform* component : _TransformComponents) {
        if (component->isChanged()) {
            component->transform();
        }
    }
}

void SSceneSystemTransform::onComponentAdded(SSceneComponent* component) {
    if (!validateComponent(component, S_SCENE_OBJECT_COM_TRANSFORM)) {
        return;
    }
    _TransformComponents.push_back(static_cast<SSceneComTransform*>(component));
}

void SSceneSystemTransform::onComponentRemoved(SSceneComponent* component) {
    for (size_t i = 0; i < _TransformComponents.size(); i++) {
        if (_TransformComponents[i] == component) {
            _TransformComponents.erase(_TransformComponents.begin() + i);
            return;
        }
    }
}

void SSceneSystemTransform::initializeLua(sol::state& luaState) {
    auto setPosFunction = sol::resolve<void(const Vector2)>(&SSceneComTransform::setPosition);
    luaState.new_usertype<SSceneComTransform>("ComTransform",
        "posAbs", sol::property(&SSceneComTransform::getPositionAbs),
        "getPosAbs", &SSceneComTransform::getPositionAbs,
        "position", sol::property(&SSceneComTransform::getPosition, setPosFunction),
        "getPosition", &SSceneComTransform::getPosition,
        "setPosition", setPosFunction,
        "x", sol::property(&SSceneComTransform::getX, &SSceneComTransform::setX),
        "y", sol::property(&SSceneComTransform::getY, &SSceneComTransform::setY),
        "z", sol::property(&SSceneComTransform::getZ, &SSceneComTransform::setZ),
        "absZ", sol::property(&SSceneComTransform::getAbsZ),
        "angle", sol::property(&SSceneComTransform::getAngle, &SSceneComTransform::setAngle),
        "setAngle", &SSceneComTransform::setAngle,
        "getAngle", &SSceneComTransform::getAngle,
        "scale", sol::property(&SSceneComTransform::getScale, &SSceneComTransform::setScale),
        "getScale", &SSceneComTransform::getScale,
        "setScale", &SSceneComTransform::setScale,
        "scaleAbs", sol::property(&SSceneComTransform::getScaleAbs),
        "inheritScale", sol::property(&SSceneComTransform::getInheritScale, &SSceneComTransform::setInheritScale),
        "parentAsPivot", sol::property(&SSceneComTransform::getParentAsPivot, &SSceneComTransform::setParentAsPivot)
    );
}

void SSceneSystemTransform::bindComponentsToLua(SSceneSystemLuaScript* luaSystem) {
    for (SSceneComTransform* com : _TransformComponents) {
        com->bindToLuaScript<SSceneComTransform*>();
    }
}