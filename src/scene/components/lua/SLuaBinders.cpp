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
        "y", &Vector2::y
    );
}

void SLuaBinders::bindInputManager(sol::state& state) {
    state.new_usertype<StormInputManager>("ModInputManager", 
        "isPointerDown", &StormInputManager::isPointerDown,
        "getPointerPosition", &StormInputManager::getPointerPosition
    );

    state["InputManager"] = StormEngine::getInputManager();
}