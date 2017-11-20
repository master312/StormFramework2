#include "SSceneComLuaScript.h"
#include "SSceneSystemLuaScript.h"
#include "StormEngine.h"
#include "scene/StormSceneObject.h"

SSceneComLuaScript::SSceneComLuaScript(StormSceneObject* owner) : SSceneComponent(owner) {
    _Type = S_SCENE_OBJECT_COM_SCRIPT;
    
    _Filename = "";
}

SSceneComLuaScript::~SSceneComLuaScript() {
}

void SSceneComLuaScript::serializeXml(pugi::xml_node& node) {
    SSceneComponent::serializeXml(node);
    node.append_attribute("filename").set_value(_Filename.c_str());
}

int SSceneComLuaScript::deserializeXml(pugi::xml_node& node) {
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

int SSceneComLuaScript::initialize(SSceneComponentSystem* system) {
    SSceneComponent::initialize(system);
    SSceneSystemLuaScript* luaSystem = dynamic_cast<SSceneSystemLuaScript*>(system);
    if (!luaSystem) {
        return -9;
    }

    sol::state& luaState = luaSystem->getLuaState();

    if (_Filename.size() <= 2) {
        LOG(ERROR) << "Could not initialize lua script. Filename not specified";
        return -1;
    }

    spStormResourceFile scriptFile = StormEngine::getResource(_Filename);
    if (!scriptFile) {
        LOG(ERROR) << "Lua script can not be initialized. File not found.";
        return -2;
    }
    
    if (scriptFile->getBufferSize() <= 4) {
        /* Script to short */
        LOG(ERROR) << "Infalid script file " << _Filename;
        return -3;
    }

    /* Creates temporary table 'this' */
    sol::table entityObject = luaState.script(scriptFile->getBuffer());
    if (!entityObject.valid()) {
        LOG(ERROR) << "Invalid entity lua table. ID: " << _Owner->getId();
        return -4;
    }

    entityObject["id"] = _Owner->getId();
    entityObject["object"] = _Owner;

    /* Adds object's table to lua handler */
    sol::function fun = luaState["createObjectHandle"];
    if (!fun.valid()) {
        LOG(ERROR) << "LUA createHandle function not found.";
        return -5;
    }
    _LuaHandler = fun(entityObject);

    if (_Owner->getIsCreatedAtRuntime()) {
        /* Lua system is already initialized. 
         * Bind all components on for SceneObject to lua */
        for (SSceneComponent* component : _Owner->getComponents()) {
            if (component == this) {
                continue;
            }
            component->bindToScript(luaSystem->getLuaState());
        }
        executeOnLoad();
        executeOnStart();
    }

    LOG(DEBUG) << "Lua script: '" << scriptFile->getFilename() << "' loaded";

    return 1;
}

void SSceneComLuaScript::executeOnLoad() {
    sol::function function = getFunction("onLoad");
    if (function.valid()) {
        function();
    }
}

void SSceneComLuaScript::executeOnStart() {
    sol::function function = getFunction("onStart");
    if (function.valid()) {
        function();
    }
}

sol::table& SSceneComLuaScript::getLuaHandle() {
#ifndef PRODUCTION
    if (!_LuaHandler.valid()) {
        LOG(ERROR) << "Getting invalid lua handler for object " << _Owner->getId();
    }
#endif
    return _LuaHandler;
}

sol::function SSceneComLuaScript::getFunction(const std::string& name) {
    sol::function function = getLuaHandle()["obj"][name];
    return function;
}

std::string& SSceneComLuaScript::getFilename() {
    return _Filename;
}