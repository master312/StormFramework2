#include "StormSceneObject.h"
#include "components/SSceneComTransform.h"
#include "components/SSceneComLuaScript.h"
#include "../core/StormCommon.h"

StormSceneObject::StormSceneObject(uint32_t id /* = 0 */) {
    _Id = id;
    _Name = "";
    _Parent = nullptr;
    _ComponentTransform = nullptr;
    _ComponentLuaScript = nullptr;
    _IsCreatedAtRuntime = false;
}

StormSceneObject::StormSceneObject(uint32_t id, const std::string& name) : 
    StormSceneObject(id) {
    _Name = name;
}

StormSceneObject::~StormSceneObject() {
    for (size_t i = 0; i < _Components.size(); i++) {
        delete _Components[i];
    }
    _Components.clear();
    _Parent = nullptr;
    _Children.clear();
}

void StormSceneObject::serializeXml(pugi::xml_node& node) {
    node.append_attribute("id").set_value(_Id);
    node.append_attribute("name").set_value(_Name.c_str());

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

int StormSceneObject::deserializeXml(pugi::xml_node& node) {
    _Id = node.attribute("id").as_int(0);
    _Name = node.attribute("name").as_string("");

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

bool StormSceneObject::getIsCreatedAtRuntime() {
    return _IsCreatedAtRuntime;
}

void StormSceneObject::setIsCreatedAtRuntime(bool value) {
    _IsCreatedAtRuntime = value;
}

void StormSceneObject::setId(uint32_t id) {
    _Id = id;
}

uint32_t StormSceneObject::getId() {
    return _Id;
}

std::string& StormSceneObject::getName() {
    return _Name;
}

void StormSceneObject::setParent(StormSceneObject* parent) {
    if (_Parent == parent) {
        LOG(DEBUG) << "ERROR: Setting same parent to object " << getId();
        return;
    }
    if (_Parent) {
        /* Object already have parent set */
        clearParent();
    }
    
    _Parent = parent;
    
    if (_Parent) {
        /* NOTE: It might be usefull to check for doubles in @_Children, or not?... */
        _Parent->_Children.push_back(this);
    }
    
    notifyObservers(S_OBSERVER_EVENT_PARENT_CHANGED);
}

StormSceneObject* StormSceneObject::getParent() {
    return _Parent;
}

std::vector<StormSceneObject*>& StormSceneObject::getChildren() {
    return _Children;
}

void StormSceneObject::setName(const std::string& name) {
    _Name = name;
}

void StormSceneObject::addComponent(SSceneComponent* component) {
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

SSceneComponent* StormSceneObject::getComponent(SSceneComponentType type) {
    for (SSceneComponent* com : _Components) {
        if (com->getType() == type) {
            return com;
        }
    }
    return nullptr;
}

std::vector<SSceneComponent*>& StormSceneObject::getComponents() {
    return _Components;
}

SSceneComTransform* StormSceneObject::getTransform() const {
#ifndef PRODUCTION
    if (!_ComponentTransform) {
        LOG(WARNING) << "Tryed to get transfrom from scene object that dose not have transform component";
        return nullptr;
    }
#endif
    return _ComponentTransform;
}

SSceneComLuaScript* StormSceneObject::getLuaScript() const {
    return _ComponentLuaScript;
}

void StormSceneObject::clearParent() {
    if (!_Parent) {
        return;
    }

    std::vector<StormSceneObject*>& children = _Parent->_Children;
    for (size_t i = 0; i < children.size(); i++) {
        if (children[i] == this) {
            children.erase(children.begin() + i);
            _Parent = nullptr;
            return;
        }
    }

    _Parent = nullptr;
}