#include "SSceneSystemLuaScript.h" 
#include "SSceneComLuaScript.h"
#include "../../core/utils/math/Vector2.h"

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
    _LuaState.open_libraries(sol::lib::base, sol::lib::os);

    /* Load lua debug methods */
    _LuaState.create_table("debug");
    _LuaState["debug"]["log"] = some_function;

    _LuaState.new_usertype<Vector2>("Vector2", 
        "x", &Vector2::x,
        "y", &Vector2::y
    );

    _LuaState.script(R"(
        Handles = {}
        
        function Vector2:_str()
            return "'X:" .. self.x .. " Y:" .. self.y .. "'"
        end

        function createObjectHandle(object)
            local handle = {
                obj = object,
                isValid = true
            }
            Handles[object.id] = handle
            return handle
        end

        function tickObjects(deltaTime) 
            for key,value in pairs(Handles) do
                local object = Handles[key]
                if object.isValid and object.obj.onUpdate ~= nil then
                    object.obj.onUpdate(deltaTime)
                end
            end
        end
    )");
    


    for (SSceneComLuaScript* component : _ScriptComponents) {
        if (component->initializeLua(_LuaState) < 0) {
            /* initializeLua() will log error */
            continue;
        }

        onScriptLoad(component);
    }
    
    for (SSceneComLuaScript* component : _ScriptComponents) {
        onScriptStart(component);
    }
}

void SSceneSystemLuaScript::tick(float deltaTime) {
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
