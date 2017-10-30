#pragma once
#include "../SSceneComponentSystem.h"
#include "sol.hpp"

/* System used for rendering planes in debug mode
 * And might be used for something more one day. */

class SSceneComLuaScript;

class SSceneSystemLuaScript : public SSceneComponentSystem {
public:
    SSceneSystemLuaScript();
    virtual ~SSceneSystemLuaScript();

    virtual void addComponent(SSceneComponent* component);

    /* Load all scripts and execute initial methods */
    virtual void initialize(StormScene* ownerScene = nullptr);
    
    virtual void tick(float deltaTime);

private:
    /* Vector containing all components. Used for faster component access. */
    std::vector<SSceneComLuaScript*> _ScriptComponents;

    /* Main lua state */
    sol::state _LuaState;

    /* Set to false when this system ticks first time
     * Used for handling @onScriptStart(...) method */
    bool _IsFirstTick;

    /* Called right after script has been loaded and initialized */
    void onScriptLoad(SSceneComLuaScript* component);
    
    /* Called on just before first tick */
    void onScriptStart(SSceneComLuaScript* component);
};
