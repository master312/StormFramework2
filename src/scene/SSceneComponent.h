#pragma once
#include "../../libs/pugixml/pugixml.hpp"

/* Base class for all scene object components.
 * Most components are managed by proper component system.
 * Some components might not even need a system, like plane component.
 * Creation of component is handled manually, while actual objects are
 * deleted when SceneObject (@_Owner) is destroyed. */

/* Enum contains type of all components */
typedef enum SSceneComponentType {
    S_SCENE_OBJECT_COM_UNDEFINED = 0,
    
    S_SCENE_OBJECT_COM_TRANSFORM,            // Transformation component
    S_SCENE_OBJECT_COM_PLANE,                // Plane component
    S_SCENE_OBJECT_COM_STATIC_TEXTURE,       // Static texture component
    S_SCENE_OBJECT_COM_SPRITE,               // Animated texture component
    S_SCENE_OBJECT_COM_SPINE,                // Spine component
    S_SCENE_OBJECT_COM_SCRIPT,

    S_SCENE_OBJECT_COM_TYPES_COUNT
};

/* Order of component type initialization */
static const int SSceneComponentInitializationOrder[S_SCENE_OBJECT_COM_TYPES_COUNT] = {
    S_SCENE_OBJECT_COM_TRANSFORM,
    S_SCENE_OBJECT_COM_PLANE,
    S_SCENE_OBJECT_COM_STATIC_TEXTURE,
    S_SCENE_OBJECT_COM_SPRITE,
    S_SCENE_OBJECT_COM_SPINE,
    S_SCENE_OBJECT_COM_SCRIPT
};

/* Contain string names of all components. */
static const std::string SSceneComponentTypeString[S_SCENE_OBJECT_COM_TYPES_COUNT] = {
    "Undefined",
    "Transform",
    "Plane",
    "Static texture",
    "Sprite",
    "Spine",
    "Script",
};

class StormSceneObject;

class SSceneComponent {
public:
    SSceneComponent(StormSceneObject* owner);
    virtual ~SSceneComponent();
    
    void setOwner(StormSceneObject* owner);
    StormSceneObject* getOwner();

    SSceneComponentType getType() const;

    /* Saves component data to @node */
    virtual void serializeXml(pugi::xml_node& node);

    /* Loads component data from @node. 
     * Returns < 0 on error */
    virtual int deserializeXml(pugi::xml_node& node);

    /* Called after all components and scene have been loaded */
    virtual void initialize();

    /* Produces new component of @SSceneComponentType and return pointer to it */
    static SSceneComponent* newComponent(SSceneComponentType type, StormSceneObject* owner);

protected:
    /* Type of scene object component */
    SSceneComponentType _Type;

    /* Scene object that is owning this component */
    StormSceneObject* _Owner;
};
