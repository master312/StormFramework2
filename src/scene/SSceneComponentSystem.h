#pragma once
#include <vector>
#include "SSceneComponent.h"
#include "StormCommon.h"
#include "sol2/sol.hpp"

/* Base class for all component systems.
 * TODO: Create system for checking for double component entrys in _Components. 
 *     : In SSceneComponentSystem::initialize method, handle errors returned by componets. */

class StormRenderer;
class SScene;

class SSceneComponentSystem {
public:
    SSceneComponentSystem(SScene* scene);
    virtual ~SSceneComponentSystem();
    
    virtual void initialize();

    virtual void render(StormRenderer* renderer);

    virtual void tick(float deltaTime);

    virtual void addComponent(SSceneComponent* component);
    
    virtual int bindToLua(sol::state& luaState);

    SSceneComponentType getType() const;

    bool getIsInitialized();

protected:
    SSceneComponentType _Type;
    
    SScene* _OwnerScene;
    
    std::vector<SSceneComponent*> _Components;
    
    bool _IsInitialized;

    /* Returns true if component is matches type, and has owner. 
     * Otherwise returns false and logs error. */
    virtual bool validateComponent(SSceneComponent* com, SSceneComponentType type);
};
