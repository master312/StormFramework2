#pragma once
#include <vector>
#include "SSceneComponent.h"
#include "../core/StormCommon.h"
#include "sol.hpp"

/* Base class for all component systems.
 * TODO: Create system for checking for double component entrys in _Components. 
 *     : In SSceneComponentSystem::initialize method, handle errors returned by componets. */

class StormRenderer;
class StormScene;

class SSceneComponentSystem {
public:
    SSceneComponentSystem();
    virtual ~SSceneComponentSystem();
    
    virtual void initialize(StormScene* ownerScene = nullptr);

    virtual void render(StormRenderer* renderer);

    virtual void tick(float deltaTime);

    virtual void addComponent(SSceneComponent* component);
    
    virtual int bindToLua(sol::state& luaState);

    SSceneComponentType getType() const;

    bool getIsInitialized();

protected:
    SSceneComponentType _Type;
    
    std::vector<SSceneComponent*> _Components;
    
    bool _IsInitialized;

    /* Returns true if component is matches type, and has owner. 
     * Otherwise returns false and logs error. */
    virtual bool validateComponent(SSceneComponent* com, SSceneComponentType type);
};
