#include "StormEngineEditing.h"
#include "scene/SSceneManager.h"
#include "scene_editing/lua_script/SESystemLuaScript.h"

StormEngineEditing::StormEngineEditing() : StormEngine() {

}

StormEngineEditing::~StormEngineEditing() {

}

StormEngineEditing* StormEngineEditing::instanceEditing() {
    return static_cast<StormEngineEditing*>(StormEngine::instance());
}

void StormEngineEditing::saveActiveScene(const std::string& path /* = "" */) {
    SScene* activeScene = _ModSceneManager->getActiveScene();
    if (!activeScene) {
        return;
    }

    activeScene->saveXml(path);
}

void StormEngineEditing::activateTool(const std::string& name) {
    SScene* activeScene = _ModSceneManager->getActiveScene();
    if (!activeScene) {
        return;
    }
    SESystemLuaScript* system = dynamic_cast<SESystemLuaScript*>(activeScene->getScriptSystem());

    sol::function selectToolFun = system->getGlobalFunction("selectTool");
    if (!selectToolFun.valid()) {
        LOG(ERROR) << "Could not select tool. Lua function not found.";
    }

    selectToolFun(name);
}