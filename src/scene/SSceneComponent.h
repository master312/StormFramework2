#pragma once
#include "pugixml/pugixml.hpp"
#include "sol2/sol.hpp"

/* Base class for all scene object components.
 * Most components are managed by proper component system.
 * Some components might not even need a system, like plane component.
 * Creation of component is handled manually, while actual objects are
 * deleted when SceneObject (@_Owner) is destroyed. */

/* Enum contains type of all components */
typedef enum {
    S_SCENE_OBJECT_COM_UNDEFINED = 0,
    
    S_SCENE_OBJECT_COM_TRANSFORM = 1,            // Transformation component
    S_SCENE_OBJECT_COM_SPRITE = 2,               // Animated texture component
    S_SCENE_OBJECT_COM_SPINE = 3,                // Spine component
    S_SCENE_OBJECT_COM_SCRIPT = 4,
    S_SCENE_OBJECT_COM_PHYSICS = 5,

    S_SCENE_OBJECT_COM_TYPES_COUNT
} SSceneComponentType;

/* Contain string names of all components. */
static const std::string SSceneComponentTypeString[S_SCENE_OBJECT_COM_TYPES_COUNT] = {
    "Undefined",
    "Transform",
    "Sprite",
    "Spine",
    "Script",
    "Physics",
};

/* Order of component type initialization */
static const int SSceneComponentInitializationOrder[S_SCENE_OBJECT_COM_TYPES_COUNT] = {
    S_SCENE_OBJECT_COM_TRANSFORM,
    S_SCENE_OBJECT_COM_SPRITE,
    S_SCENE_OBJECT_COM_SPINE,
    S_SCENE_OBJECT_COM_PHYSICS,
    /* Make sure script is always last */
    S_SCENE_OBJECT_COM_SCRIPT
};


/* Count is defined here because we might need some systems ticked more then once */
static const int SSceneComponentTickingOrderCount = 6;
/* Order in which component systems will tick. */
static const int SSceneComponentTickingOrder[SSceneComponentTickingOrderCount] = {
    S_SCENE_OBJECT_COM_TRANSFORM,
    S_SCENE_OBJECT_COM_PHYSICS,
    S_SCENE_OBJECT_COM_SPRITE,
    S_SCENE_OBJECT_COM_SPINE,
    S_SCENE_OBJECT_COM_SCRIPT,
};



class SSceneObject;
class SSceneComponentSystem;

class SSceneComponent {
public:
    SSceneComponent(SSceneObject* owner);
    virtual ~SSceneComponent();
    
    void setOwner(SSceneObject* owner);
    SSceneObject* getOwner();

    SSceneComponentType getType() const;

    /* Saves component data to @node */
    virtual void serializeXml(pugi::xml_node& node);

    /* Loads component data from @node. 
     * Returns < 0 on error */
    virtual int deserializeXml(pugi::xml_node& node);

    /* Called after all components and scene have been loaded */
    virtual int initialize(SSceneComponentSystem* system);

    /* Returns whether component is initialized or not */
    bool getIsInitialized();

    /* Has this component been binded to script */
    bool getIsBindedToScript();

    /* Should be set to true after component gets binded to script */
    void setIsBindedToScript(bool value);

    /* Returns owner's lua handle table */
    sol::table getOwnerLuaHandle();

    /* Returns reference to LUA handle name for this SSceneComponent */
    std::reference_wrapper<const std::string> getLuaHandleName();

    /* Produces new component of @SSceneComponentType and return pointer to it */
    static SSceneComponent* newComponent(SSceneComponentType type, SSceneObject* owner);

protected:
    /* Type of scene object component */
    SSceneComponentType _Type;

    /* Scene object that is owning this component */
    SSceneObject* _Owner;

    bool _IsInitialized;

    /* Name of this component in script.
     * EX: this._ScriptHandlerName */
    std::string _ScriptHandlerName; 

    /* Has this component been binded to script */
    bool _IsBindedToScript;
};
