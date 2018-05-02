#pragma once
#include "scene/SSceneComponentSystem.h"
#include "sol2/sol.hpp"

class SComLuaScript;
class SSceneObject;

class SSystemLuaScript : public SSceneComponentSystem {
#ifdef STORM_EDITOR
    friend class SESystemLuaScript;
#endif

public:
    SSystemLuaScript(SScene* scene);
    virtual ~SSystemLuaScript();

    virtual void addComponent(SSceneComponent* component);

    /* Load all scripts and execute initial methods */
    virtual void initialize();
    
    virtual void tick(float deltaTime);

    /* Gets lua handle for scene object */
    sol::table getObjectHandle(uint32_t id);

    /* Creates script handle for scene object @object. */
    void registerSceneObjectHandle(SSceneObject* object);

    /* Destroys object handle. After executing this method object will be
     * inaccessible from script */
    void destroyObjectHandle(SSceneObject* object);

private:
    /* Vector containing all components. Used for faster component access. */
    std::vector<SComLuaScript*> _ScriptComponents;

    /* Main lua state reference */
    sol::state& _LuaState;

    /* Set to false when this system ticks first time
     * Used for handling @onScriptStart(...) method */
    bool _IsFirstTick;

    /* Load common scripts.
     * @fileList are script paths from the root of the filesystem. */
    virtual void loadCommonScripts(const std::vector<std::string>& fileList);

    /* Bind other systems to lua and create object handles for all scene objects*/
    virtual void initializeScene();
};
