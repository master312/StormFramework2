#pragma once
#include <vector>
#include <map>
#include <functional>
#include "SSceneComponent.h"
#include "SSceneObjectEventTypes.h"
#include "../core/utils/SObservable.h"

class SSceneComTransform;

class StormSceneObject : public SObservable<SSceneObjectEventType, SSceneComponent> {
public:
    StormSceneObject(uint32_t id = 0);
    StormSceneObject(uint32_t id, const std::string& name);
    virtual ~StormSceneObject();

    /* Saves object and all it's component data to @node */
    void serializeXml(pugi::xml_node& node);

    /* Loads object and components from @node. 
     * Returns < 0 on error */
    int deserializeXml(pugi::xml_node& node);
    
    /* Sets object's unique identifier */
    void setId(uint32_t id);

    /* Gets object's unique identifier */
    uint32_t getId();

    /* Assign name to the object */
    void setName(const std::string& name);

    /* Gets object name */
    std::string& getName();

    /* Assign parent to object, or clear parent if @parent = nullptr.
     * If there is already a parent set, it will be replased */
    void setParent(StormSceneObject* parent);

    /* Return parent of this object, or nullptr if none */
    StormSceneObject* getParent();

    /* Returns vector of all child objects */
    std::vector<StormSceneObject*>& getChildren();

    /* Adds component to the object */
    void addComponent(SSceneComponent* component);

    /* Returns component of specefied type */
    SSceneComponent* getComponent(SSceneComponentType type);

    /* Returns reference to vector containing all components */
    std::vector<SSceneComponent*>& getComponents();

    /* Returns pointer to transform component, or nullptr if none exists */
    SSceneComTransform* getTransform() const;

private:
    /* Unique object identifier */
    uint32_t _Id;

    /* Object name */
    std::string _Name;

    /* Pointer to parent object */
    StormSceneObject* _Parent;

    /* Vector of all children child objects */
    std::vector<StormSceneObject*> _Children;

    /* All components attached to object */
    std::vector<SSceneComponent*> _Components;

    /* Pointer to transform component */
    SSceneComTransform* _ComponentTransform;

    /* Removes this object from parent's @_Components array and clears @_Parent pointer */
    void clearParent();
};
