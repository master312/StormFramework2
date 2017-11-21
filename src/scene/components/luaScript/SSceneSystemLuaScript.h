#pragma once
#include "../../SSceneComponentSystem.h"
#include "sol.hpp"

class SSceneComLuaScript;
class StormSceneObject;

class SSceneSystemLuaScript : public SSceneComponentSystem {
public:
    SSceneSystemLuaScript();
    virtual ~SSceneSystemLuaScript();

    virtual void addComponent(SSceneComponent* component);

    /* Load all scripts and execute initial methods */
    virtual void initialize(StormScene* ownerScene = nullptr);
    
    virtual void tick(float deltaTime);

    sol::state& getLuaState();

    /* Gets lua handle for scene object */
    sol::table getObjectHandle(uint32_t id);

    /* Creates script handle for scene object @object. */
    void registerSceneObjectHandle(StormSceneObject* object);

private:
    /* Vector containing all components. Used for faster component access. */
    std::vector<SSceneComLuaScript*> _ScriptComponents;

    /* Main lua state */
    sol::state _LuaState;

    /* Set to false when this system ticks first time
     * Used for handling @onScriptStart(...) method */
    bool _IsFirstTick;
};
