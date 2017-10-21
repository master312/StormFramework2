#include "SSceneSystemLuaScript.h" 
#include "SSceneComLuaScript.h"
#include "lua/SLuaBinders.h"
#include "../StormScene.h"
#include "../../StormEngine.h"
#include "../../core/utils/math/Vector2.h"

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
    //SSceneComponentSystem::initialize();
    _LuaState.open_libraries(sol::lib::base, sol::lib::os);

    if (SLuaBinders::bind(_LuaState) < 0) {
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

    for (SSceneComLuaScript* component : _ScriptComponents) {
        if (component->initializeLua(_LuaState) < 0) {
            /* initializeLua() will log error */
            continue;
        }
    }
    
    for (SSceneComponentSystem* system : ownerScene->getSystems()) {
        if (system == this) {
            continue;
        }
        if (system->bindToLua(_LuaState) < 0) {
            LOG(ERROR) << "System could not be binded to lua script";
        }
    }
    
    for (SSceneComLuaScript* component : _ScriptComponents) {
        onScriptLoad(component);
    }
}

void SSceneSystemLuaScript::tick(float deltaTime) {
    if (_IsFirstTick) {
        /* Execute script onStart method */
        _IsFirstTick = false;
        for (SSceneComLuaScript* component : _ScriptComponents) {
            onScriptStart(component);
        }
    }

    _LuaState["tickObjects"](deltaTime);
}

void SSceneSystemLuaScript::addComponent(SSceneComponent* component) {
    SSceneComponentSystem::addComponent(component);
    _ScriptComponents.push_back(dynamic_cast<SSceneComLuaScript*>(component));
}

void SSceneSystemLuaScript::onScriptLoad(SSceneComLuaScript* component) {
    sol::function function = component->getLuaHandle()["obj"]["onLoad"];
    if (function.valid()) {
        function();
    }
}

void SSceneSystemLuaScript::onScriptStart(SSceneComLuaScript* component) {
    sol::function function = component->getLuaHandle()["obj"]["onStart"];
    if (function.valid()) {
        function();
    }
}
