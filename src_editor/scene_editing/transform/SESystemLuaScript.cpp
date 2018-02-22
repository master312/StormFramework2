#include "SESystemLuaScript.h"
#include "StormEngine.h"
#include "scene/components/luaScript/SSceneComLuaScript.h"

SESystemLuaScript::SESystemLuaScript(SScene* scene) : SSceneSystemLuaScript(scene) {

}

SESystemLuaScript::~SESystemLuaScript() {

}

void SESystemLuaScript::initialize() {
    initializeLua();

    StormFileSystem* fileSystem = StormEngine::getModule<StormFileSystem>();
    loadCommonScripts(fileSystem->getFilesList("lua_common/", "lua"));
    loadCommonScripts(fileSystem->getFilesList("lua_editor/common/", "lua"));

    initializeScene();

    for (SSceneComLuaScript* component : _ScriptComponents) {
        component->executeOnLoad();
    }
}