#pragma once
#include <string>
#include <vector>
#include "SSceneComponent.h"
#include "SSceneObjectTransform.h"

class StormSceneObject {
public:
    StormSceneObject(uint32_t id = 0);
    StormSceneObject(uint32_t id, const std::string& name);
    ~StormSceneObject();

    /* Saves object and all it's component data to @node */
    void serializeXml(pugi::xml_node& node);

    /* Loads object and components from @node. 
     * Returns < 0 on error */
    int deserializeXml(pugi::xml_node& node);

    /* Sets object's unique identifier */
    void setId(uint32_t id);

    /* Gets object's unique identifier */
    uint32_t getId();

    /* Gets object name */
    std::string& getName();

    /* Assign name to the object */
    void setName(const std::string& name);

    /* Returns transform info (position, rotation, scale) */
    SSceneObjectTransform& getTransform();

    /* Adds component to the object */
    void addComponent(SSceneComponent* component);

    /* Returns component of specefied type */
    SSceneComponent* getComponent(SSceneComponentType type);

    /* Returns reference to vector containing all components */
    std::vector<SSceneComponent*>& getComponents();

private:
    /* Unique object identifier */
    uint32_t _Id;

    /* Object name */
    std::string _Name;

    /* Objects transform info (position, rotation, scale) */
    SSceneObjectTransform _Transform;

    /* All components attached to object */
    std::vector<SSceneComponent*> _Components;
};
