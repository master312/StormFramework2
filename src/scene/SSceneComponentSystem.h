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

    /* Removes component from the system */
    void removeComponent(SSceneComponent* component);

    /* Adds component to system */
    void addComponent(SSceneComponent* component);

    virtual void initialize();

    virtual void render(StormRenderer* renderer);

    virtual void tick(float deltaTime);

    /* Called when component has been removed from the system */
    virtual void onComponentRemoved(SSceneComponent* component);

    /* Called when new component has been added to the system */
    virtual void onComponentAdded(SSceneComponent* component);

    SSceneComponentType getType() const;

    /* Called from SSceneSystemLua class to create LUA user type(s) for the system. */
    virtual void initializeLua(sol::state& luaState);

    /* Binds all components to owner's LUA object (if any).
     * Must be overriden to work. */
    virtual void bindComponentsToLua(SSceneSystemLuaScript* luaSystem);

    bool getIsInitialized();

    /* Returns true if component is matches type, and has owner.
     * Otherwise returns false and logs error. */
    static bool validateComponent(SSceneComponent* com, SSceneComponentType type);

    /* Creates new system object of @type */
    static SSceneComponentSystem* createSystem(SSceneComponentType type, SScene* scene);

#ifndef PRODUCTION
    /* If this method returns true, system's ticking will not be disable using debug system.*/
    virtual bool ignoreDebugDisabling();
#endif

protected:
    SSceneComponentType _Type;
    
    SScene* _OwnerScene;
    
    std::vector<SSceneComponent*> _Components;
    
    bool _IsInitialized;

#ifndef PRODUCTION
    /* If set to true, debug 'disable system ticking' option will be ignored for this system. */
    bool _IgnoreDebugDisable;
#endif
};
