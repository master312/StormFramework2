#pragma once
#include <vector>
#include "SSceneComponent.h"
#include "../core/StormCommon.h"

/* Base class for all component systems.
 * TODO: Create system for checking for double component entrys in _Components. */

class StormRenderer;

class SSceneComponentSystem {
public:
    SSceneComponentSystem();
    virtual ~SSceneComponentSystem();
    
    virtual void initialize();

    virtual void render(StormRenderer* renderer);

    virtual void tick(float deltaTime);

    virtual void addComponent(SSceneComponent* component);

    SSceneComponentType getType() const;

protected:
    SSceneComponentType _Type;
    
    std::vector<SSceneComponent*> _Components;
    
    /* Returns true if component is matches type, and has owner. 
     * Otherwise returns false and logs error. */
    virtual bool validateComponent(SSceneComponent* com, SSceneComponentType type);
};
