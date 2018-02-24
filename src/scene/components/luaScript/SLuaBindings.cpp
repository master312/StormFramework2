#include "SLuaBindings.h"
#include "StormEngine.h"
#include "scene/SScene.h"
#include "../transform/SSceneComTransform.h"
#include "../../SSceneComponent.h"

SLuaBindings::SLuaBindings() {
}

SLuaBindings::~SLuaBindings() {
}

int SLuaBindings::bindUserTypes(sol::state &state) {
    //state.create_table("debug");
    state["debug"]["log"] = SLuaBindings::debugLog;

    bindVector2(state);
    bindScene(state);
    bindInputManager(state);
    bindSceneObjectComponentBase(state);
    bindSceneObject(state);

    return 0;
}

void SLuaBindings::bindSceneObject(sol::state& state) {
    state.new_usertype<SSceneObject>("SceneObject",
        "id", sol::property(&SSceneObject::getId),
        "name", sol::property(&SSceneObject::getName, &SSceneObject::setName),
        "parent", sol::property(&SSceneObject::getParent, &SSceneObject::setParent),
        "transform", sol::property(&SSceneObject::getTransform)
    );
}

void SLuaBindings::bindSceneObjectComponentBase(sol::state& state) {
    state.new_usertype<SSceneComponent>("SceneCom",
        "type", sol::property(&SSceneComponent::getType),
        "owner", sol::property(&SSceneComponent::getOwner)
    );
}

void SLuaBindings::debugLog(const std::string& msg) {
    LOG(INFO) << "LUA: " << msg;
}

void SLuaBindings::bindVector2(sol::state& state) {
    state.new_usertype<Vector2>("Vector2",
        sol::constructors<Vector2(), Vector2(float, float)>(),
        "x", &Vector2::x,
        "y", &Vector2::y,
        "toString", &Vector2::toString
    );
}

void SLuaBindings::bindScene(sol::state& state) {
    state.new_usertype<SScene>("Scene",
        "name", sol::property(&SScene::getName),
        /* Do not use 'intInstantiatePrefab' method. Use method defined in lua script */
        "intInstantiatePrefab", &SScene::instantiatePrefab,
        /* Do not use 'intDestroyObject' method. Use method defined in lua script */
        "intDestroyObject", &SScene::destroyObject,
        "getObjectById", &SScene::getObjectById
    );
    
    state["ActiveScene"] = StormEngine::getActiveScene();
}

void SLuaBindings::bindInputManager(sol::state& state) {
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