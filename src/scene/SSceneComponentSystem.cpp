#include "SSceneComponentSystem.h"
#include "graphics/StormRenderer.h"

SSceneComponentSystem::SSceneComponentSystem(SScene* scene) {
    _Type = S_SCENE_OBJECT_COM_UNDEFINED;
    _IsInitialized = false;
    _OwnerScene = scene;

#ifndef PRODUCTION
    _IgnoreDebugDisable = false;
#endif
}

SSceneComponentSystem::~SSceneComponentSystem() {
}

void SSceneComponentSystem::initialize() {
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
    onComponentAdded(component);
}

void SSceneComponentSystem::removeComponent(SSceneComponent* component) {
    for (size_t i = 0; i < _Components.size(); i++) {
        if (_Components[i] == component) {
            _Components.erase(_Components.begin() + i);
            onComponentRemoved(component);
            return;
        }
    }
}

void SSceneComponentSystem::onComponentRemoved(SSceneComponent* component) {
}

void SSceneComponentSystem::onComponentAdded(SSceneComponent* component) {
}

void SSceneComponentSystem::initializeLua(sol::state& luaState) {
}

void SSceneComponentSystem::bindComponentsToLua(SSceneSystemLuaScript* luaSystem) {
}

SSceneComponentType SSceneComponentSystem::getType() const {
    return _Type;
}

bool SSceneComponentSystem::getIsInitialized() {
    return _IsInitialized;
}

#ifndef PRODUCTION
bool SSceneComponentSystem::ignoreDebugDisabling() {
    return _IgnoreDebugDisable;
}
#endif

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