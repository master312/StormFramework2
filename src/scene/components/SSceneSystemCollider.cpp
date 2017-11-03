#include "SSceneSystemCollider.h"
#include "SSceneComCollider.h"
#include "../StormSceneObject.h"

SSceneSystemCollider::SSceneSystemCollider() {
    _Type = S_SCENE_OBJECT_COM_COLLIDER;
}

SSceneSystemCollider::~SSceneSystemCollider() {   
}

void SSceneSystemCollider::addComponent(SSceneComponent* component) {
    SSceneComponentSystem::addComponent(component);

    _ColliderComponents.push_back(dynamic_cast<SSceneComCollider*>(component));
}

void SSceneSystemCollider::render(StormRenderer* renderer) {
    
}

int SSceneSystemCollider::bindToLua(sol::state& luaState) {
    luaState.new_usertype<SSceneComCollider>("ComCollider",
        "containsPoint", &SSceneComCollider::containsPoint
    );

    for (SSceneComCollider* com : _ColliderComponents) {
        sol::table handle = luaState["Handles"][com->getOwner()->getId()];
        if (!handle.valid() || !handle || !handle["isValid"]) {
            continue;
        }

        handle["obj"]["collider"] = com;
    }

    return 1;
}
