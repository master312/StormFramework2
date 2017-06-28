#include "SSceneComponentSystem.h"
#include "../core/graphics/StormRenderer.h"

SSceneComponentSystem::SSceneComponentSystem() {
    _Type = S_SCENE_OBJECT_COM_SYS_UNDEFINED;
}

SSceneComponentSystem::~SSceneComponentSystem() {
}

void SSceneComponentSystem::render(StormRenderer* renderer) {
}

void SSceneComponentSystem::tick(float deltaTime) {
}

SSceneComponentSystemType SSceneComponentSystem::getType() const {
    return _Type;
}

bool SSceneComponentSystem::validateComponent(SSceneComponent* com, SSceneComponentType type) {
    if (com->getType() != type) {
        LOG(WARNING) << "ValidateComponent failed. Type missmatch!";
        return false;
    }
    if (!com->getOwner()) {
        LOG(WARNING) << "ValidateComponent failed. Owner not set!";
        return false;
    }
    return true;
}