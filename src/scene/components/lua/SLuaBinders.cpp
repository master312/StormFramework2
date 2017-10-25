#include "SLuaBinders.h"
#include "../../SSceneComponentSystem.h"
#include "../../../StormEngine.h"
#include "../../../core/utils/math/Vector2.h"

SLuaBinders::SLuaBinders() {
}

SLuaBinders::~SLuaBinders() {
}

int SLuaBinders::bind(sol::state& state) { 
    state.create_table("debug");
    state["debug"]["log"] = SLuaBinders::debugLog;

    bindVector2(state);
    bindInputManager(state);

    return 0;
}

void SLuaBinders::debugLog(const std::string& msg) {
    LOG(INFO) << "LUA: " << msg;
}

void SLuaBinders::bindVector2(sol::state& state) {
    state.new_usertype<Vector2>("Vector2", 
        "x", &Vector2::x,
        "y", &Vector2::y,
        "toString", &Vector2::toString
    );
}

void SLuaBinders::bindInputManager(sol::state& state) {
    /* Bind all keys to lua */
    sol::table KeyboardLuaTable = state.create_table("Keyboard");
    for (uint32_t i = 0; i < (uint32_t)S_KEY_ASCII_END; i++) {
        const char* keyString = StormInputManager::keyToString((StormKey)(i));
        if (!keyString) {
            continue;
        }
        KeyboardLuaTable[keyString] = (StormKey)i;
        
    }
    
    state.new_usertype<StormInputManager>("ModInputManager", 
        "isPointerDown", &StormInputManager::isPointerDown,
        "getPointerPosition", &StormInputManager::getPointerPosition,
        "isKeyDown", &StormInputManager::isKeyDown,
        "isKeyPressed", &StormInputManager::isKeyPressed
    );

    
    state["InputManager"] = StormEngine::getInputManager();
}