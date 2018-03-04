#include "SSceneObject.h"
#include "StormCommon.h"
#include "SScene.h"
#include "components/transform/SSceneComTransform.h"
#include "components/luaScript/SSceneSystemLuaScript.h"
#include "components/luaScript/SSceneComLuaScript.h"
#ifdef STORM_EDITOR
#include "scene_editing/lua_script/SESystemLuaScript.h"
#endif

SSceneObject::SSceneObject(SScene* scene, uint32_t id /* = 0 */) {
    _Scene = scene;
    _Id = id;
    _Name = "";
    _Parent = nullptr;
    _ComponentTransform = nullptr;
    _ComponentLuaScript = nullptr;
    _IsCreatedAtRuntime = false;
}

SSceneObject::SSceneObject(SScene* scene, uint32_t id, const std::string& name) :
    SSceneObject(scene, id) {
    _Name = name;
    StormMiscTools::trimSpacesBeginEnd(_Name);
}

SSceneObject::~SSceneObject() {
    clearParent();
    clearChildren();
    for (size_t i = 0; i < _Components.size(); i++) {
        delete _Components[i];
    }
    _Components.clear();
    _Scene = nullptr;
}

void SSceneObject::serializeXml(pugi::xml_node& node) {
    node.append_attribute("id").set_value(_Id);
    if (_Name != "") {
        node.append_attribute("name").set_value(_Name.c_str());
    }
    if (_Parent) {
        node.append_attribute("parent").set_value(_Parent->getId());
    }

    if (!_Components.size()) {
        /* Object dose not have any components */
        return;
    }
    
    for (SSceneComponent* component : _Components) {
        pugi::xml_node componentNode = node.append_child("component");
        component->serializeXml(componentNode);
    }

    LOG(DEBUG) << "Object id:" << _Id << " saved to xml";
}

int SSceneObject::deserializeXml(pugi::xml_node& node) {
    _Id = node.attribute("id").as_int(0);
    _Name = node.attribute("name").as_string("");
    StormMiscTools::trimSpacesBeginEnd(_Name);

    /* Load components */
    for (pugi::xml_node comNode = node.first_child(); comNode; comNode = comNode.next_sibling()) {
        if (comNode.type() != pugi::node_element) {
            continue;
        }
        SSceneComponentType type = (SSceneComponentType)comNode.attribute("type").as_int(0);
        SSceneComponent* component = SSceneComponent::newComponent(type, this);
        if (!component) {
            /* Error is logged in @SSceneComponent::newComponent */
            continue;
        }
        component->deserializeXml(comNode);
        addComponent(component);
    }

    return 1;
}

bool SSceneObject::getIsCreatedAtRuntime() {
    return _IsCreatedAtRuntime;
}

void SSceneObject::setIsCreatedAtRuntime(bool value) {
    _IsCreatedAtRuntime = value;
}

void SSceneObject::setId(uint32_t id) {
    _Id = id;
}

uint32_t SSceneObject::getId() {
    return _Id;
}

std::reference_wrapper<const std::string> SSceneObject::getName() const {
    return _Name;
}

void SSceneObject::setParent(SSceneObject* parent) {
    if (_Parent == parent) {
        LOG(DEBUG) << "ERROR: Setting same parent to object " << getId();
        return;
    }
    if (_Parent && parent) {
        /* Object already have parent set */
        clearParent(true);
    }
    
    _Parent = parent;
    
    if (_Parent) {
        /* NOTE: It might be usefull to check for doubles in @_Children, or not?... */
        _Parent->_Children.push_back(this);
    }
    
    notifyObservers(S_OBSERVER_EVENT_PARENT_CHANGED);
}

SSceneObject* SSceneObject::getParent() {
    return _Parent;
}

std::vector<SSceneObject*>& SSceneObject::getChildren() {
    return _Children;
}

void SSceneObject::setName(const std::string& name) {
    _Name = name;
    StormMiscTools::trimSpacesBeginEnd(_Name);
}

void SSceneObject::addComponent(SSceneComponent* component) {
    _Components.push_back(component);

    switch (component->getType()) {
        case S_SCENE_OBJECT_COM_TRANSFORM:
            if (_ComponentTransform) {
                LOG(ERROR) << "Adding multiple transform components to same scene object";
            }
            _ComponentTransform = dynamic_cast<SSceneComTransform*>(component);
            break;
        case S_SCENE_OBJECT_COM_SCRIPT:
            _ComponentLuaScript = dynamic_cast<SSceneComLuaScript*>(component);
            break;
        default:
            break;
    }
}

SSceneComponent* SSceneObject::getComponent(SSceneComponentType type) {
    for (SSceneComponent* com : _Components) {
        if (com->getType() == type) {
            return com;
        }
    }
    return nullptr;
}

std::vector<SSceneComponent*>& SSceneObject::getComponents() {
    return _Components;
}

SSceneComTransform* SSceneObject::getTransform() const {
#ifndef PRODUCTION
    if (!_ComponentTransform) {
        LOG(WARNING) << "Tryed to get transfrom from scene object that dose not have transform component";
        return nullptr;
    }
#endif
    return _ComponentTransform;
}

SSceneComLuaScript* SSceneObject::getLuaScript() const {
    return _ComponentLuaScript;
}

sol::table SSceneObject::getLuaHandle() {
    SSceneSystemLuaScript* luaSystem = _Scene->getScriptSystem();
    return luaSystem->getObjectHandle(_Id);
}

sol::function SSceneObject::getLuaFunction(const std::string& functionName) {
#ifdef STORM_EDITOR
    SESystemLuaScript* scriptSystem = dynamic_cast<SESystemLuaScript*>(_Scene->getScriptSystem());
    if (scriptSystem && !scriptSystem->getTickGameScripts()) {
        /* Game script ticking disabled */
        return sol::function();
    }
#endif

    sol::function function = getLuaHandle()[functionName];
    if (!function.valid()) {
        LOG(WARNING) << "SSceneObject::getLuaFunction could not find function '" << functionName << "'";
    }
    return function;
}

SScene* SSceneObject::getScene() const {
    return _Scene;
}

void SSceneObject::clearParent(bool dontNotifyObervers /* = false */) {
    if (!_Parent) {
        return;
    }

    std::vector<SSceneObject*>& children = _Parent->_Children;
    for (size_t i = 0; i < children.size(); i++) {
        if (children[i] == this) {
            children.erase(children.begin() + i);
            break;
        }
    }

    if (!dontNotifyObervers) {
        notifyObservers(S_OBSERVER_EVENT_PARENT_CHANGED);
    }
    _Parent = nullptr;
}

void SSceneObject::clearChildren() {
    for (size_t i = 0; i < _Children.size(); i++) {
        _Children[i]->setParent(nullptr);
    }
    _Children.clear();
}