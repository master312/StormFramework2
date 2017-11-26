#include "SSceneSystemTransform.h"
#include "SSceneComTransform.h"
#include "scene/StormSceneObject.h"

SSceneSystemTransform::SSceneSystemTransform(StormScene* scene) : SSceneComponentSystem(scene){
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

void SSceneSystemTransform::addComponent(SSceneComponent* component) {
    if (!validateComponent(component, S_SCENE_OBJECT_COM_TRANSFORM)) {
        return;
    }
    SSceneComponentSystem::addComponent(component);

    SSceneComTransform* transform = dynamic_cast<SSceneComTransform*>(component);
    if (!transform) {
        LOG(ERROR) << "Could not cast component to SSceneComTransform";
        return;
    }

    _TransformComponents.push_back(transform);
}

int SSceneSystemTransform::bindToLua(sol::state& luaState) {
    auto setPosFunction = sol::resolve<void(const Vector2)>(&SSceneComTransform::setPosition);
    luaState.new_usertype<SSceneComTransform>("ComTransform",
        "posAbs", sol::property(&SSceneComTransform::getPositionAbs),
        "position", sol::property(&SSceneComTransform::getPosition, setPosFunction),
        "getPosition", &SSceneComTransform::getPosition,
        "setPosition", setPosFunction,
        "x", sol::property(&SSceneComTransform::getX, &SSceneComTransform::setX),
        "y", sol::property(&SSceneComTransform::getY, &SSceneComTransform::setY),
        "angle", sol::property(&SSceneComTransform::getAngle, &SSceneComTransform::setAngle),
        "inheritScale", sol::property(&SSceneComTransform::getInheritScale, &SSceneComTransform::setInheritScale),
        "parentAsPivot", sol::property(&SSceneComTransform::getParentAsPivot, &SSceneComTransform::setParentAsPivot)
    );
    
    for (SSceneComTransform* com : _TransformComponents) {
        com->bindToScript(luaState);
    }

    return 1;
}