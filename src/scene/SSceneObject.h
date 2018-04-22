#pragma once
#include <vector>
#include <map>
#include <functional>
#include "SSceneComponent.h"
#include "SSComponentEvents.h"
#include "utils/SNotificationCenter.h"

class SComTransform;
class SComLuaScript;
class SScene;

/* Define scene object events structure */
namespace SEventDispatcher {
    struct SSceneObjectEvent : public Event {
        enum Types {
            ADDED = S_GENERATE_EVENT_ID('s', 'o', 'a'),
            PREFAB_INSTANTIATED = S_GENERATE_EVENT_ID('o', 'p', 'i'),
#ifdef STORM_EDITOR
            EDIT_OBJECT_SELECTED = SE_GENERATE_EVENT_ID('o', 'e', 's'),
#endif
        };
        SSceneObject* object;
        SSceneObjectEvent(SSceneObject* obj, int32_t type = ADDED)
                : Event(type), object(obj) { }
    };
};

class SSceneObject : public SNotificationCenter<SSComponentEvents> {
public:
    SSceneObject(SScene* scene, uint32_t id = 0);
    SSceneObject(SScene* scene, uint32_t id, const std::string& name);
    virtual ~SSceneObject();

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
    std::reference_wrapper<const std::string> getName() const;

    /* Assign parent to object, or clear parent if @parent = nullptr.
     * If there is already a parent set, it will be replased */
    void setParent(SSceneObject* parent);

    /* Return parent of this object, or nullptr if none */
    SSceneObject* getParent();

    /* Returns vector of all child objects */
    std::vector<SSceneObject*>& getChildren();

    /* Adds component to the object */
    void addComponent(SSceneComponent* component);

    void setIsCreatedAtRuntime(bool value);

    /* Returns true if this object has been created 
     * at runtime, and not loaded from scene file */
    bool getIsCreatedAtRuntime();
    
    /* Returns component of specefied type */
    SSceneComponent* getComponent(SSceneComponentType type);

    /* Returns reference to vector containing all components */
    std::vector<SSceneComponent*>& getComponents();

    /* Returns pointer to transform component, or nullptr if none exists
     * Used for easy access to transform component, since its used offen */
    SComTransform* getTransform() const;

    /* Returns pointer to lua component. Used for faster access.
     * Retunrs nullptr if there is no lua component. */
    SComLuaScript* getLuaScript() const;

    /* Returns lua handle for this scene object */
    sol::table getLuaHandle();

    /* Returns lua function from lua script for this object */
    sol::function getLuaFunction(const std::string& functionName);

    /* Returns scene which owns this object */
    SScene* getScene() const;

    /* Unlinks object from parent (if any).
     * Removes this object from parent's @_Children array and clears @_Parent pointer.
     * If @dontNotifyObervers is set to true, "ParentRemoved" event wont be fired.*/
    void clearParent(bool dontNotifyObervers = false);

    /* Unlink every child from this element */
    void clearChildren();

private:
    /* Unique object identifier */
    uint32_t _Id;

    /* Object name */
    std::string _Name;

    /* Pointer to object's owner scene */
    SScene* _Scene;

    /* Pointer to parent object */
    SSceneObject* _Parent;

    /* Vector of all children child objects */
    std::vector<SSceneObject*> _Children;

    /* All components attached to object */
    std::vector<SSceneComponent*> _Components;

    /* Pointer to transform component */
    SComTransform* _ComponentTransform;

    /* Pointer to lua script component */
    SComLuaScript* _ComponentLuaScript;
 
    /* Set to true if this object is created at runtime,
     * and not loaded from scene file */
    bool _IsCreatedAtRuntime;
};

