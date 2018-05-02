#include "SSystemLuaScript.h"
#include "SComLuaScript.h"
#include "lua/SLuaSystem.h"
#include "StormEngine.h"
#include "scene/SSceneObject.h"
#include "scene/SScene.h"
/*
    Component load script and holds pointer to table
*/

SSystemLuaScript::SSystemLuaScript(SScene* scene) :
        SSceneComponentSystem(scene),
        _LuaState(StormEngine::getLua()->getState()) {

    _Type = S_SCENE_OBJECT_COM_SCRIPT;
    _IsFirstTick = true;
}

SSystemLuaScript::~SSystemLuaScript() {
}

void SSystemLuaScript::initialize() {
    /* TODO: Read common files path from some config file */
    StormFileSystem* fileSystem = StormEngine::getModule<StormFileSystem>();
    loadCommonScripts(fileSystem->getFilesList("lua_common/", "lua"));

    initializeScene();

    for (SComLuaScript* component : _ScriptComponents) {
        component->executeOnLoad();
    }
}

void SSystemLuaScript::loadCommonScripts(const std::vector<std::string>& fileList) {
    for (const std::string& filename : fileList) {
        spStormResourceFile resFile = StormEngine::getResource(filename);
        if (!resFile) {
            LOG(ERROR) << "Failed to load common lua script '" << filename << "'";
            continue;
        }
        /* Load common script */
        _LuaState.script(resFile->getBuffer());
        LOG(DEBUG) << "Common lua script '" << resFile << "' loaded";
    }
}

void SSystemLuaScript::initializeScene() {
    /* Bind all scene objects to script */
    for (SSceneObject* object : _OwnerScene->getObjects()) {
        registerSceneObjectHandle(object);
    }

    SSceneComponentSystem::initialize();

    for (size_t i = 0; i < S_SCENE_OBJECT_COM_TYPES_COUNT; i++) {
        SSceneComponentSystem* system = _OwnerScene->getSystemByType((SSceneComponentType)i);
        if (!system || system == this) {
            continue;
        }

        /* Bind usertype to lua */
        system->initializeLua(_LuaState);
        /* Bind all components to owner's script object. */
        system->bindComponentsToLua(this);
    }
}

void SSystemLuaScript::tick(float deltaTime) {
    if (_IsFirstTick) {
        /* Execute script onStart method */
        _IsFirstTick = false;
        for (SComLuaScript* component : _ScriptComponents) {
            component->executeOnStart();
        }
    }

    _LuaState["tickObjects"](deltaTime);
}

sol::table SSystemLuaScript::getObjectHandle(uint32_t id) {
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

void SSystemLuaScript::registerSceneObjectHandle(SSceneObject* object) {
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

void SSystemLuaScript::destroyObjectHandle(SSceneObject* object) {
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

void SSystemLuaScript::addComponent(SSceneComponent* component) {
    SSceneComponentSystem::addComponent(component);
    _ScriptComponents.push_back(dynamic_cast<SComLuaScript*>(component));
}
