#include "SLuaBinders.h"
#include "StormEngine.h"
#include "scene/SScene.h"
#include "scene/SSceneObject.h"
#include "scene/SSceneComponentSystem.h"
#include "utils/math/Vector2.h"
#include "../transform/SSceneComTransform.h"

SLuaBinders::SLuaBinders() {
}

SLuaBinders::~SLuaBinders() {
}

int SLuaBinders::bindStandardTypes(sol::state& state) { 
    //state.create_table("debug");
    state["debug"]["log"] = SLuaBinders::debugLog;

    bindVector2(state);
    bindScene(state);
    bindInputManager(state);

    return 0;
}

void SLuaBinders::bindSceneObject(sol::state& state) {
    state.new_usertype<SSceneObject>("SceneObject",
        "id", sol::property(&SSceneObject::getId),
        "name", sol::property(&SSceneObject::getName, &SSceneObject::setName),
        "parent", sol::property(&SSceneObject::getParent, &SSceneObject::setParent),
        "transform", sol::property(&SSceneObject::getTransform)
    );
}

void SLuaBinders::debugLog(const std::string& msg) {
    LOG(INFO) << "LUA: " << msg;
}

void SLuaBinders::bindVector2(sol::state& state) {
    state.new_usertype<Vector2>("Vector2",
        sol::constructors<Vector2(), Vector2(float, float)>(),
        "x", &Vector2::x,
        "y", &Vector2::y,
        "toString", &Vector2::toString
    );
}



void SLuaBinders::bindScene(sol::state& state) {
    state.new_usertype<SScene>("Scene",
        "name", sol::property(&SScene::getName),
        /* Do not use this method. Use method defined in lua script */
        "intInstantiatePrefab", &SScene::instantiatePrefab,
        "destroyObject", &SScene::destroyObject,
        "getObjectById", &SScene::getObjectById
    );
    
    state["ActiveScene"] = StormEngine::getActiveScene();
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