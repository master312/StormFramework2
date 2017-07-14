#pragma once
#include <vector>
#include <map>
#include <functional>
#include "SSceneComponent.h"
#include "SSceneObjectEventTypes.h"

/* Struct holds info about observers from components that are listening to events fired from scene object */
struct SSceneObjectEventObserver {
    SSceneComponent* component;
    SSceneObjectEventType type;
    std::function<void(void*)> callbackMethod;

    SSceneObjectEventObserver() : component(nullptr), 
                                  type(S_OBSERVER_EVENT_UNDEFINED), callbackMethod(nullptr) { }

    SSceneObjectEventObserver(SSceneComponent* _component, SSceneObjectEventType _type, std::function<void(void*)> _callback) 
                                : component(_component), type(_type), callbackMethod(_callback) { }
};

/* Macro for easy generation of event observers. */
#define S_OBJECT_ADD_OBSERVER(object, ptr, type, method) object->registerEventObserver(SSceneObjectEventObserver(ptr, type, std::bind(method, ptr, std::placeholders::_1)));
#define S_OBJECT_REMOVE_OBSERVERS(object) object->removeEventObserver(this);

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

    /* Adds component to the object */
    void addComponent(SSceneComponent* component);

    /* Returns component of specefied type */
    SSceneComponent* getComponent(SSceneComponentType type);

    /* Returns reference to vector containing all components */
    std::vector<SSceneComponent*>& getComponents();

    /* This method will fire event to all registered observers.
     * Mostly fired by components of this object. */
    void fireEvent(SSceneObjectEventType type);

    /* Register observer that will be notified if event of certen type occures */
    void registerEventObserver(SSceneObjectEventObserver observer);

    /* Removes event observer.
     * If @type is set to S_OBSERVER_EVENT_UNDEFINED, every observer by @component will be removed */
    void removeEventObserver(SSceneComponent* component, SSceneObjectEventType type = S_OBSERVER_EVENT_UNDEFINED);

private:
    /* Unique object identifier */
    uint32_t _Id;

    /* Object name */
    std::string _Name;

    /* All components attached to object */
    std::vector<SSceneComponent*> _Components;

    /* Map of all registered event observers, indexed by type 
     * <EventType> <AllCallbackMethods> */
    std::map<SSceneObjectEventType, std::vector<SSceneObjectEventObserver> > _Observers;

    /* Removes every event observer by @component from @vector */
    void removeEventObserverFromVector(SSceneComponent* component, std::vector<SSceneObjectEventObserver>& vector);
};
