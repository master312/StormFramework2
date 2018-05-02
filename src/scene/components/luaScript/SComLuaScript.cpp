#include "SComLuaScript.h"
#include "SSystemLuaScript.h"
#include "StormEngine.h"
#include "scene/SSceneObject.h"
#include "lua/SLuaSystem.h"

SComLuaScript::SComLuaScript(SSceneObject* owner) : SSceneComponent(owner) {
    _Type = S_SCENE_OBJECT_COM_SCRIPT;
    
    _Filename = "";
}

SComLuaScript::~SComLuaScript() {
}

void SComLuaScript::serializeXml(pugi::xml_node& node) {
    SSceneComponent::serializeXml(node);
    node.append_attribute("filename").set_value(_Filename.c_str());
}

int SComLuaScript::deserializeXml(pugi::xml_node& node) {
    if (SSceneComponent::deserializeXml(node) < 0) {
        return -1;
    }
    
    _Filename = node.attribute("filename").as_string();
    if (_Filename.size() <= 2) {
        LOG(ERROR) << "Lua script component dose not have filename specified";
        return -1;
    }

    return 1;
}

int SComLuaScript::initialize(SSceneComponentSystem* system) {
    SSceneComponent::initialize(system);
    SSystemLuaScript* luaSystem = dynamic_cast<SSystemLuaScript*>(system);
    if (!luaSystem) {
        return -9;
    }

    sol::state& luaState = StormEngine::getLua()->getState();

    if (_Filename.size() <= 2) {
        LOG(ERROR) << "Could not initialize lua script. Filename not specified";
        return -1;
    }

    /* Creates temporary table 'this' */
    sol::table scriptContent = StormEngine::getLua()->loadToTable(
            StormEngine::getResource(_Filename));
    if (!scriptContent.valid()) {
        LOG(ERROR) << "Invalid lua script table content for entity ID: " << _Owner->getId();
        return -4;
    }

    if (_Owner->getIsCreatedAtRuntime()) {
        /* Object is created at runtime. We need new lua handle now */
        luaSystem->registerSceneObjectHandle(_Owner);
    }

    _LuaHandle = luaSystem->getObjectHandle(_Owner->getId());

    if (!_LuaHandle || !_LuaHandle.valid()) {
        LOG(ERROR) << "Invalid lua handle for object ID: " << _Owner->getId();
        return -5;
    }

    /* Bind script to handle */
    _LuaHandle["script"] = scriptContent;
    
    /* Execute method to let the lua know that script has just been loaded */
    sol::function handleOnScriptLoaded = luaState["handleOnScriptLoaded"];
    if (handleOnScriptLoaded && handleOnScriptLoaded.valid()) {
        handleOnScriptLoaded(scriptContent, _LuaHandle);
    } else {
        LOG(ERROR) << "Lua function 'handleOnScriptLoaded' not found!";
    }

    if (_Owner->getIsCreatedAtRuntime()) {
        /* Object is created on runtime. So execute on load now */
        executeOnLoad();
        /* TODO: Don't execute OnStart here. Execute if on first next update() */
        executeOnStart();
    }

    LOG(DEBUG) << "Lua script: '" << _Filename << "' loaded";

    return 1;
}

void SComLuaScript::executeOnLoad() {
    sol::function function = getFunction("onLoad");
    if (function.valid()) {
        function();
    }
}

void SComLuaScript::executeOnStart() {
    sol::function function = getFunction("onStart");
    if (function.valid()) {
        function();
    }
}

sol::table& SComLuaScript::getLuaHandle() {
#ifndef PRODUCTION
    if (!_LuaHandle.valid()) {
    LOG(ERROR) << "Getting invalid lua handler for object " << _Owner->getId();
    }
#endif
    return _LuaHandle;
}

sol::function SComLuaScript::getFunction(const std::string& name) {
    sol::function function = getLuaHandle()[name];
    return function;
}

std::string& SComLuaScript::getFilename() {
    return _Filename;
}