#include "SSceneComponent.h"
#include "SSceneObject.h"
#include "scene/components/sprite/SComSprite.h"
#include "scene/components/transform/SComTransform.h"
#include "scene/components/luaScript/SComLuaScript.h"
#include "scene/components/physics/SComPhysics.h"
#include "scene/SScene.h"
#include "SSceneComponentSystem.h"

SSceneComponent::SSceneComponent(SSceneObject* owner) {
    _Type = S_SCENE_OBJECT_COM_UNDEFINED;
    _Owner = owner;
    _IsInitialized = false;
    _IsBindedToScript = false;
    _ScriptHandlerName = "";
}

SSceneComponent::~SSceneComponent() {
    _Owner->removeObservers(this);
    SSceneComponentSystem* system = _Owner->getScene()->getSystemByType(_Type);
    if (system) {
        system->removeComponent(this);
    }
    _Owner = nullptr;
    _IsInitialized = false;
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

void SSceneComponent::setOwner(SSceneObject* owner) {
    if (_Owner) {
        LOG(WARNING) << "Changing owner of scene component. This is not supported and might fail";
    }
    _Owner = owner;
}

SSceneObject* SSceneComponent::getOwner() {
    return _Owner;
}

void SSceneComponent::onLuaBinded(bool hasScript) {
}

/* Static component producer method */
SSceneComponent* SSceneComponent::newComponent(SSceneComponentType type, SSceneObject* owner) {
    SSceneComponent* component = nullptr;
    switch(type) {
        case S_SCENE_OBJECT_COM_TRANSFORM:
            component = new SComTransform(owner);
            break;
        case S_SCENE_OBJECT_COM_PHYSICS:
            component = new SComPhysics(owner);
            break;
        case S_SCENE_OBJECT_COM_SPRITE:
            component = new SComSprite(owner);
            break;
        case S_SCENE_OBJECT_COM_SCRIPT:
            component = new SComLuaScript(owner);
            break;
        default:
            LOG(ERROR) << "Tryed to create new component of invalid type " << type;
            break;
    }

    return component;
}
