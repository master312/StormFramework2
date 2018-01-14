#pragma once
#include <vector>
#include "SSceneComponent.h"
#include "StormCommon.h"
#include "sol2/sol.hpp"

/* Base class for all component systems.
 * TODO: Create system for checking for double component entrys in _Components. 
 *     : In SSceneComponentSystem::initialize method, handle errors returned by components. */
class StormRenderer;
class SScene;
class SSceneSystemLuaScript;

class SSceneComponentSystem {
public:
    SSceneComponentSystem(SScene* scene);
    virtual ~SSceneComponentSystem();
    
    virtual void initialize();

    virtual void render(StormRenderer* renderer);

    virtual void tick(float deltaTime);

    virtual void addComponent(SSceneComponent* component);

    SSceneComponentType getType() const;

    /* Called from SSceneSystemLua class to create LUA user type(s) for the system. */
    virtual void initializeLua(sol::state& luaState);

    /* Binds all components to owner's LUA object (if any).
     * Must be overriden to work. */
    virtual void bindComponentsToLua(SSceneSystemLuaScript* luaSystem);

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
