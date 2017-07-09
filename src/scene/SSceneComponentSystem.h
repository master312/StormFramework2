#pragma once
#include <vector>
#include "SSceneComponent.h"
#include "../core/StormCommon.h"

/* Base class for all component systems.
 * TODO: Create system for checking for double component entrys in _Components. */

typedef enum SSceneComponentSystemType {
    /* Enum contains type of all components managing systems */
    S_SCENE_OBJECT_COM_SYS_UNDEFINED = 0,

    S_SCENE_OBJECT_COM_SYS_PLANE,
    S_SCENE_OBJECT_COM_SYS_STATIC_TEXTURE,      // Static texture component
    S_SCENE_OBJECT_COM_SYS_SPRITE,              // Animated texture component
    S_SCENE_OBJECT_COM_SYS_SPINE,               // Spine component
};

class StormRenderer;

class SSceneComponentSystem {
public:
    SSceneComponentSystem();
    virtual ~SSceneComponentSystem();
    
    virtual void render(StormRenderer* renderer);

    virtual void tick(float deltaTime);

    virtual void addComponent(SSceneComponent* component) = 0;

    SSceneComponentSystemType getType() const;

protected:
    SSceneComponentSystemType _Type;
    
    /* Returns true if component is matches type, and has owner. 
     * Otherwise returns false and logs error. */
    virtual bool validateComponent(SSceneComponent* com, SSceneComponentType type);
};
