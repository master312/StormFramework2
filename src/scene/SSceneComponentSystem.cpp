#include "SSceneComponentSystem.h"
#include "graphics/StormRenderer.h"

#include "components/physics/SSceneSystemPhysics.h"
#include "components/sprite/SSceneSystemSprite.h"
#include "components/transform/SSceneSystemTransform.h"
#include "components/luaScript/SSceneSystemLuaScript.h"
#ifdef STORM_EDITOR
#include "scene_editing/transform/SESystemTransform.h"
#include "scene_editing/lua_script/SESystemLuaScript.h"
#endif

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

SSceneComponentSystem* SSceneComponentSystem::createSystem(SSceneComponentType type,
                                                           SScene* scene) {
    switch (type) {
        case S_SCENE_OBJECT_COM_TRANSFORM:
#ifdef STORM_EDITOR
            return new SESystemTransform(scene);
#else
            return new SSceneSystemTransform(scene);
#endif
        case S_SCENE_OBJECT_COM_SPRITE:
            return new SSceneSystemSprite(scene);
        case S_SCENE_OBJECT_COM_SCRIPT:
#ifdef STORM_EDITOR
            return new SESystemLuaScript(scene);
#else
            return new SSceneSystemLuaScript(scene);
#endif
        case S_SCENE_OBJECT_COM_PHYSICS:
            return new SSceneSystemPhysics(scene);
        default:
            LOG(ERROR) << "Could not create system of type " << SSceneComponentTypeString[type];
            break;
    }

    return nullptr;
}