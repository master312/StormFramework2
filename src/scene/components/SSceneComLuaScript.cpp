#include "SSceneComLuaScript.h"
#include "../StormSceneObject.h"
#include "../../StormEngine.h"

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

int SSceneComLuaScript::initializeLua(sol::state& luaState) {
    SSceneComponent::initialize();
    
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
    entityObject["hasParent"] = _Owner->getParent() ? true : false;
    if (_Owner->getParent()) {
        entityObject["parent"] = luaState["Handles"][_Owner->getParent()->getId()];
    }

    /* Adds object's table to lua handler */
    sol::function fun = luaState["createObjectHandle"];
    if (!fun.valid()) {
        LOG(ERROR) << "LUA createHandle function not found.";
        return -5;
    }
    _LuaHandler = fun(entityObject);

    LOG(DEBUG) << "Lua script: '" << scriptFile->getFilename() << "' loaded";

    return 1;
}

sol::table& SSceneComLuaScript::getLuaHandle() {
#ifdef _DEBUG
    if (!_LuaHandler.valid()) {
        LOG(ERROR) << "Getting invalid lua handler for object " << _Owner->getId();
    }
#endif
    return _LuaHandler;
}

std::string& SSceneComLuaScript::getFilename() {
    return _Filename;
}