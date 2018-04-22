#include "SSystemTransform.h"
#include "SComTransform.h"
#include "scene/SScene.h"
#include "scene/components/luaScript/SSystemLuaScript.h"

SSystemTransform::SSystemTransform(SScene* scene) : SSceneComponentSystem(scene) {
    _Type = S_SCENE_OBJECT_COM_TRANSFORM;
}

SSystemTransform::~SSystemTransform() {
}

void SSystemTransform::tick(float deltaTime) {
    for (SComTransform* component : _TransformComponents) {
        if (component->isChanged()) {
            component->transform();
        }
    }
}

void SSystemTransform::onComponentAdded(SSceneComponent* component) {
    if (!validateComponent(component, S_SCENE_OBJECT_COM_TRANSFORM)) {
        return;
    }
    _TransformComponents.push_back(static_cast<SComTransform*>(component));
}

void SSystemTransform::onComponentRemoved(SSceneComponent* component) {
    for (size_t i = 0; i < _TransformComponents.size(); i++) {
        if (_TransformComponents[i] == component) {
            _TransformComponents.erase(_TransformComponents.begin() + i);
            return;
        }
    }
}

void SSystemTransform::initializeLua(sol::state& luaState) {
    auto setPosFunction = sol::resolve<void(const Vector2)>(&SComTransform::setPosition);
    luaState.new_usertype<SComTransform>("ComTransform",
        "posAbs", sol::property(&SComTransform::getPositionAbs),
        "getPosAbs", &SComTransform::getPositionAbs,
        "position", sol::property(&SComTransform::getPosition, setPosFunction),
        "getPosition", &SComTransform::getPosition,
        "setPosition", setPosFunction,
        "x", sol::property(&SComTransform::getX, &SComTransform::setX),
        "y", sol::property(&SComTransform::getY, &SComTransform::setY),
        "z", sol::property(&SComTransform::getZ, &SComTransform::setZ),
        "absZ", sol::property(&SComTransform::getAbsZ),
        "angle", sol::property(&SComTransform::getAngle, &SComTransform::setAngle),
        "setAngle", &SComTransform::setAngle,
        "getAngle", &SComTransform::getAngle,
        "scale", sol::property(&SComTransform::getScale, &SComTransform::setScale),
        "getScale", &SComTransform::getScale,
        "setScale", &SComTransform::setScale,
        "scaleAbs", sol::property(&SComTransform::getScaleAbs),
        "inheritScale", sol::property(&SComTransform::getInheritScale, &SComTransform::setInheritScale),
        "parentAsPivot", sol::property(&SComTransform::getParentAsPivot, &SComTransform::setParentAsPivot)
    );
}

void SSystemTransform::bindComponentsToLua(SSystemLuaScript* luaSystem) {
    for (SComTransform* com : _TransformComponents) {
        com->bindToLuaScript<SComTransform*>();
    }
}