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
    virtual void initialize();
    
    virtual void tick(float deltaTime);

private:
    /* Vector containing all components. Used for faster component access. */
    std::vector<SSceneComLuaScript*> _ScriptComponents;

    /* Main lua state */
    sol::state _LuaState;
    
    /* Called right after script has been loaded */
    void onScriptLoad(SSceneComLuaScript* component);
    /* Called after all script have been loaded and game is about to start */
    void onScriptStart(SSceneComLuaScript* component);
};
