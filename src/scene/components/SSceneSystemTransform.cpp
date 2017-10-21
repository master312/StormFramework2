#include "SSceneSystemTransform.h"
#include "SSceneComTransform.h"
#include "../StormSceneObject.h"

SSceneSystemTransform::SSceneSystemTransform() {
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
    luaState.new_usertype<SSceneComTransform>("ComTransform",
        "posAbs", sol::property(&SSceneComTransform::getPositionAbs),
        "getPosition", &SSceneComTransform::getPosition,
        "setPosition", &SSceneComTransform::setPosition,
        "x", sol::property(&SSceneComTransform::getX, &SSceneComTransform::setX),
        "y", sol::property(&SSceneComTransform::getY, &SSceneComTransform::setY),
        "angle", sol::property(&SSceneComTransform::getAngle, &SSceneComTransform::setAngle)
    );
    
    for (SSceneComTransform* com : _TransformComponents) {
        sol::table handle = luaState["Handles"][com->getOwner()->getId()];
        if (!handle.valid() || !handle || !handle["isValid"]) {
            continue;
        }

        handle["obj"]["transform"] = com;
    }

    return 1;
}