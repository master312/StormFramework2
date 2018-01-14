#include "SSceneComponent.h"
#include "SSceneObject.h"
#include "components/sprite/SSceneComSprite.h"
#include "components/transform/SSceneComTransform.h"
#include "components/luaScript/SSceneComLuaScript.h"
#include "components/physics/SSceneComPhysics.h"
#include "core/StormCommon.h"
#include "SSceneComponentSystem.h"

SSceneComponent::SSceneComponent(SSceneObject* owner) {
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
    return 0;
}

int SSceneComponent::initialize(SSceneComponentSystem* system) {
    _IsInitialized = true;
    return 1;
}

bool SSceneComponent::getIsInitialized() {
    return _IsInitialized;
}

sol::table SSceneComponent::getOwnerLuaHandle() {
    return _Owner->getLuaHandle();
}

std::reference_wrapper<const std::string> SSceneComponent::getLuaHandleName() {
    return _ScriptHandlerName;
}

bool SSceneComponent::getIsBindedToScript() {
     return _IsBindedToScript;
}

void SSceneComponent::setIsBindedToScript(bool value) {
    _IsBindedToScript = value;
}

void SSceneComponent::setOwner(SSceneObject* owner) {
    if (_Owner) {
        LOG(WARNING) << "Changing owner of scene component. This is not supported and might fail";
    }
    _Owner = owner;
}

SSceneObject* SSceneComponent::getOwner() {
    return _Owner;
}

/* Static component producer method */
SSceneComponent* SSceneComponent::newComponent(SSceneComponentType type, SSceneObject* owner) {
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
