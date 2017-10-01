#include "SSceneComponent.h"
#include "StormSceneObject.h"
#include "components/SSceneComPlane.h"
#include "components/SSceneComStaticTexture.h"
#include "components/SSceneComTransform.h"
#include "components/SSceneComLuaScript.h"
#include "../core/StormCommon.h"

SSceneComponent::SSceneComponent(StormSceneObject* owner) {
    _Type = S_SCENE_OBJECT_COM_UNDEFINED;
    _Owner = owner;
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

void SSceneComponent::initialize() {
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

int SSceneComponent::bindToLua(sol::table& object) {
    return 0;
}

/* Static component producer method */
SSceneComponent* SSceneComponent::newComponent(SSceneComponentType type, StormSceneObject* owner) {
    SSceneComponent* component = nullptr;
    switch(type) {
        case S_SCENE_OBJECT_COM_TRANSFORM:
            component = new SSceneComTransform(owner);
            break;
        case S_SCENE_OBJECT_COM_PLANE:
            component = new SSceneComPlane(owner);
            break;
        case S_SCENE_OBJECT_COM_STATIC_TEXTURE:
            component = new SSceneComStaticTexture(owner);
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
