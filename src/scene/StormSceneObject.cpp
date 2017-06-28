#include "StormSceneObject.h"
#include "../core/StormCommon.h"

StormSceneObject::StormSceneObject(uint32_t id /* = -1 */) {
    _Id = id;
    _Name = "";
}

StormSceneObject::StormSceneObject(uint32_t id /* = 0 */, const std::string& name) {
    _Id = id;
    _Name = name;
}

StormSceneObject::~StormSceneObject() {
    for (unsigned int i = 0; i < _Components.size(); i++) {
        delete _Components[i];
    }
    _Components.clear();
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

void StormSceneObject::setId(uint32_t id) {
    _Id = id;
}

uint32_t StormSceneObject::getId() {
    return _Id;
}

std::string& StormSceneObject::getName() {
    return _Name;
}

void StormSceneObject::setName(const std::string& name) {
    _Name = name;
}

void StormSceneObject::addComponent(SSceneComponent* component) {
    _Components.push_back(component);   
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