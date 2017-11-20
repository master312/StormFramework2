#include "SSceneComponentSystem.h"
#include "graphics/StormRenderer.h"

SSceneComponentSystem::SSceneComponentSystem() {
    _Type = S_SCENE_OBJECT_COM_UNDEFINED;
    _IsInitialized = false;
}

SSceneComponentSystem::~SSceneComponentSystem() {
}

void SSceneComponentSystem::initialize(StormScene* ownerScene /* = nullptr*/) {
    for (SSceneComponent* component : _Components) {
        component->initialize(this);
    }
    _IsInitialized = true;
}

void SSceneComponentSystem::render(StormRenderer* renderer) {
}

void SSceneComponentSystem::tick(float deltaTime) {
}

void SSceneComponentSystem::addComponent(SSceneComponent* component) {
    _Components.push_back(component);
}

int SSceneComponentSystem::bindToLua(sol::state& luaState) {
    return 0;
}

SSceneComponentType SSceneComponentSystem::getType() const {
    return _Type;
}

bool SSceneComponentSystem::getIsInitialized() {
    return _IsInitialized;
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