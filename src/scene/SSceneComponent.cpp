#include "SSceneComponent.h"
#include "StormSceneObject.h"
#include "components/SSceneComSprite.h"
#include "components/SSceneComTransform.h"
#include "components/SSceneComLuaScript.h"
#include "components/SSceneComPhysics.h"
#include "SSceneComponentSystem.h"
#include "../core/StormCommon.h"

SSceneComponent::SSceneComponent(StormSceneObject* owner) {
    _Type = S_SCENE_OBJECT_COM_UNDEFINED;
    _Owner = owner;
    _IsInitialized = false;
    _IsBindedToScript = false;
    _ScriptHandlerName = "";
}

SSceneComponent::~SSceneComponent() {
    S_OBSERVER_REMOVE_ALL(_Owner, this);
    _Owner = nullptr;
}

SSceneComponentType SSceneComponent::getType() const {
    return _Type;
}

void SSceneComponent::serializeXml(pugi::xml_node& node) {
    node.append_attribute("type").set_value((int)_Type);
}

int SSceneComponent::deserializeXml(pugi::xml_node& node) {
}

int SSceneComponent::initialize(SSceneComponentSystem* system) {
    _IsInitialized = true;
    return 1;
}

bool SSceneComponent::getIsInitialized() {
    return _IsInitialized;
}

void SSceneComponent::bindToScript(sol::state& luaState) {
    LOG(ERROR) << "SSceneComponent::bindToScript Must be overriden!";
}

sol::table SSceneComponent::getOwnerLuaHandle(sol::state& luaState) {
    if (!getOwner()->getLuaScript()) {
        /* Parent object dose not have script attatched */
        return sol::table();
    }

    if (_ScriptHandlerName == "") {
        LOG(ERROR) << "Could not bind component to script. Script handler name is not set";
        return sol::table();
    }
    
    sol::table handle = luaState["Handles"][getOwner()->getId()];
    if (!handle.valid() || !handle || !handle["isValid"]) {
        LOG(ERROR) << "Component tryed to bind to invalid script object handle";
        return sol::table();
    }

    return handle;
}

bool SSceneComponent::getIsBindedToScript() {
     return _IsBindedToScript;
}

void SSceneComponent::setOwner(StormSceneObject* owner) {
    if (_Owner) {
        LOG(WARNING) << "Changing owner of scene component. This is not supported and might fail";
    }
    _Owner = owner;
}

StormSceneObject* SSceneComponent::getOwner() {
    return _Owner;
}

/* Static component producer method */
SSceneComponent* SSceneComponent::newComponent(SSceneComponentType type, StormSceneObject* owner) {
    SSceneComponent* component = nullptr;
    switch(type) {
        case S_SCENE_OBJECT_COM_TRANSFORM:
            component = new SSceneComTransform(owner);
            break;
        case S_SCENE_OBJECT_COM_PHYSICS:
            component = new SSceneComPhysics(owner);
            break;
        case S_SCENE_OBJECT_COM_SPRITE:
            component = new SSceneComSprite(owner);
            break;
        case S_SCENE_OBJECT_COM_SCRIPT:
            component = new SSceneComLuaScript(owner);
            break;
        default:
            LOG(ERROR) << "Tryed to create new component of invalid type " << type;
            break;
    }

    return component;
}
