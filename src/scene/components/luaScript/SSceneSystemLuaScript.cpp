#include "SSceneSystemLuaScript.h" 
#include "SSceneComLuaScript.h"
#include "SLuaBinders.h"
#include "StormEngine.h"
#include "scene/StormSceneObject.h"
#include "scene/StormScene.h"
#include "utils/math/Vector2.h"

/*
    Component load script and holds pointer to table
*/

SSceneSystemLuaScript::SSceneSystemLuaScript() {
    _Type = S_SCENE_OBJECT_COM_SCRIPT;
    _IsFirstTick = true;
}

SSceneSystemLuaScript::~SSceneSystemLuaScript() {
}

void SSceneSystemLuaScript::initialize(StormScene* ownerScene) {
    _LuaState.open_libraries(sol::lib::base, sol::lib::os, 
                             sol::lib::math, sol::lib::io,
                             sol::lib::count, sol::lib::package);

    if (SLuaBinders::bindStandardTypes(_LuaState) < 0) {
        LOG(ERROR) << "Could not bind lua functions";
        return;
    }

    spStormResourceFile resFile = StormEngine::getResource("lua_common/lua_common.lua");
    if (!resFile) {
        LOG(FATAL) << "Failed to initialize script engine. No common scripts found";
        return;
    }

    /* Load common script(s) */
    _LuaState.script(resFile->getBuffer());
    
    SLuaBinders::bindSceneObject(_LuaState);

    /* Bind all scene objects to script */
    for (StormSceneObject* object : ownerScene->getObjects()) {
        registerSceneObjectHandle(object);
    }

    SSceneComponentSystem::initialize(ownerScene);
    
    for (SSceneComponentSystem* system : ownerScene->getSystems()) {
        if (system == this) {
            continue;
        }
        if (system->bindToLua(_LuaState) < 0) {
            LOG(ERROR) << "System could not be binded to lua script";
        }
    }

    for (SSceneComLuaScript* component : _ScriptComponents) {
        component->executeOnLoad();
    }
}

void SSceneSystemLuaScript::tick(float deltaTime) {
    if (_IsFirstTick) {
        /* Execute script onStart method */
        _IsFirstTick = false;
        for (SSceneComLuaScript* component : _ScriptComponents) {
            component->executeOnStart();
        }
    }

    _LuaState["tickObjects"](deltaTime);
}

sol::state& SSceneSystemLuaScript::getLuaState() {
    return _LuaState;
}

sol::table SSceneSystemLuaScript::getObjectHandle(uint32_t id) {
#ifndef PRODUCTION
    sol::table handle = _LuaState["Handles"][id];
    if (!handle || !handle.valid()) {
        LOG(WARNING) << "Getting non existing lua handler for object ID: " << id;
    }
    return handle;
#else
    return _LuaState["Handles"][id];
#endif
}

void SSceneSystemLuaScript::registerSceneObjectHandle(StormSceneObject* object) {
    if (!object) {
        LOG(ERROR) << "Tryed to create lua handle for null object";
        return;
    }
    sol::function fun = _LuaState["createObjectHandle"];
    if (!fun.valid()) {
        LOG(FATAL) << "LUA createHandle function not found.";
        return;
    }
    fun(object);
}

void SSceneSystemLuaScript::addComponent(SSceneComponent* component) {
    SSceneComponentSystem::addComponent(component);
    _ScriptComponents.push_back(dynamic_cast<SSceneComLuaScript*>(component));
}
