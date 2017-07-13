#include "StormSceneObject.h"
#include "../core/StormCommon.h"

StormSceneObject::StormSceneObject(uint32_t id /* = 0 */) {
    _Id = id;
    _Name = "";
    _Position.set(0.0f, 0.0f);
}

StormSceneObject::StormSceneObject(uint32_t id, const std::string& name) : 
    StormSceneObject(id) {
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
    node.append_attribute("position_x").set_value(_Position.x);
    node.append_attribute("position_y").set_value(_Position.y);

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
    _Position.x = node.attribute("position_x").as_float(0.0f);
    _Position.y = node.attribute("position_y").as_float(0.0f);

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

void StormSceneObject::setPosition(Vector2 position) {
    _Position = position;
}

Vector2 StormSceneObject::getPosition() {
    return _Position;
}

Vector2* StormSceneObject::getPositionPtr() {
    return &_Position;
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