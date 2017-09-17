#include "SSceneSystemLuaScript.h" 
#include "SSceneComLuaScript.h"
/*
    Component load script and holds pointer to table

*/

SSceneSystemLuaScript::SSceneSystemLuaScript() {
    _Type = S_SCENE_OBJECT_COM_SCRIPT;
}

SSceneSystemLuaScript::~SSceneSystemLuaScript() {

}

void some_function(std::string a) {
    LOG(INFO) << "LUA: " << a;
}

void SSceneSystemLuaScript::initialize() {
    //SSceneComponentSystem::initialize();
    _LuaState.open_libraries(sol::lib::base);

    /* Load lua debug methods */
    _LuaState.create_table("debug");
    _LuaState["debug"]["log"] = some_function;

    _LuaState.script(R"(
        Handles = {}
        
        function createHandle(object)
            local handle = {
                obj = object,
                isValid = true
            }
            Handles[object.id] = handle
            return handle
        end
    )");
    

    int internalCnt = 0;
    for (SSceneComLuaScript* component : _ScriptComponents) {
        internalCnt++;
        if (component->initializeLua(_LuaState, internalCnt) < 0) {
            /* initializeLua() will log error */
            continue;
        }

        onScriptLoad(component);
    }
    
    for (SSceneComLuaScript* component : _ScriptComponents) {
        onScriptStart(component);
    }
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
