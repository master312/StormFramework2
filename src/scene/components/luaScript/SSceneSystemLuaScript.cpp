#include "SSceneSystemLuaScript.h" 
#include "SSceneComLuaScript.h"
#include "SLuaBinders.h"
#include "StormEngine.h"
#include "scene/SSceneObject.h"
#include "scene/SScene.h"
/*
    Component load script and holds pointer to table
*/

SSceneSystemLuaScript::SSceneSystemLuaScript(SScene* scene) : SSceneComponentSystem(scene) {
    _Type = S_SCENE_OBJECT_COM_SCRIPT;
    _IsFirstTick = true;
}

SSceneSystemLuaScript::~SSceneSystemLuaScript() {
}

void SSceneSystemLuaScript::initialize() {
    _LuaState.open_libraries(sol::lib::base, sol::lib::os, 
                             sol::lib::math, sol::lib::io,
                             sol::lib::count, sol::lib::package,
                             sol::lib::string, sol::lib::table,
                             sol::lib::debug);

    /* Sets root path for lua require() function */
    StormFileSystem* fileSystem = StormEngine::getModule<StormFileSystem>();
    std::string path = _LuaState["package"]["path"];
    _LuaState["package"]["path"] = path + ";" + fileSystem->getRootPath() + "/?.lua";

    if (SLuaBinders::bindStandardTypes(_LuaState) < 0) {
        LOG(ERROR) << "Could not bind lua functions";
        return;
    }

    std::vector<std::string> commonList = fileSystem->getFilesList("lua_common/", "lua");
    for (std::string& filename : commonList) {
        spStormResourceFile resFile = StormEngine::getResource(filename);
        if (!resFile) {
            LOG(ERROR) << "Failed to load common lua script '" << filename << "'";
            continue;
        }
        /* Load common script */
        _LuaState.script(resFile->getBuffer());
        LOG(DEBUG) << "Common lua script '" << resFile << "' loaded";
    }
    
    SLuaBinders::bindSceneObject(_LuaState);

    /* Bind all scene objects to script */
    for (SSceneObject* object : _OwnerScene->getObjects()) {
        registerSceneObjectHandle(object);
    }

    SSceneComponentSystem::initialize();
    
    for (SSceneComponentSystem* system : _OwnerScene->getSystems()) {
        if (system == this) {
            continue;
        }

        /* Bind usertype to lua */
        system->initializeLua(_LuaState);
        /* Bind all components to owner's script object. */
        system->bindComponentsToLua(this);
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

void SSceneSystemLuaScript::registerSceneObjectHandle(SSceneObject* object) {
    if (!object) {
        LOG(ERROR) << "Tried to create lua handle for null object";
        return;
    }
    sol::function fun = _LuaState["createObjectHandle"];
    if (!fun.valid()) {
        LOG(FATAL) << "LUA createObjectHandle function not found.";
        return;
    }
    fun(object);
}

void SSceneSystemLuaScript::destroyObjectHandle(SSceneObject* object) {
    if (!object) {
        return;
    }
    sol::function fun = _LuaState["destroyObjectHandle"];
    if (!fun.valid()) {
        LOG(FATAL) << "LUA destroyObjectHandle function not found.";
        return;
    }
    fun(object);
}

void SSceneSystemLuaScript::addComponent(SSceneComponent* component) {
    SSceneComponentSystem::addComponent(component);
    _ScriptComponents.push_back(dynamic_cast<SSceneComLuaScript*>(component));
}
