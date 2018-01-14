#pragma once
#include "../../SSceneComponentSystem.h"
#include "sol2/sol.hpp"

class SSceneComLuaScript;
class SSceneObject;

class SSceneSystemLuaScript : public SSceneComponentSystem {
public:
    SSceneSystemLuaScript(SScene* scene);
    virtual ~SSceneSystemLuaScript();

    virtual void addComponent(SSceneComponent* component);

    /* Load all scripts and execute initial methods */
    virtual void initialize();
    
    virtual void tick(float deltaTime);

    sol::state& getLuaState();

    /* Gets lua handle for scene object */
    sol::table getObjectHandle(uint32_t id);

    /* Creates script handle for scene object @object. */
    void registerSceneObjectHandle(SSceneObject* object);

    /* Binds SSceneComponent to owner's object lua handle */
    template <class T>
    void bindComponentToObject(SSceneComponent* component) {
        if (!component || component->getIsBindedToScript()) return;
        sol::table handle = component->getOwnerLuaHandle();
        if (!handle.valid() || !handle["script"].valid()) return;
        const std::string& str = component->getLuaHandleName();
        handle["script"][str] = dynamic_cast<T>(component);
        component->setIsBindedToScript(true);
    }

private:
    /* Vector containing all components. Used for faster component access. */
    std::vector<SSceneComLuaScript*> _ScriptComponents;

    /* Main lua state */
    sol::state _LuaState;

    /* Set to false when this system ticks first time
     * Used for handling @onScriptStart(...) method */
    bool _IsFirstTick;
};
