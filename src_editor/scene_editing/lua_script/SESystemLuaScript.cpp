#include "SESystemLuaScript.h"
#include "StormEngine.h"
#include "scene/components/luaScript/SSceneComLuaScript.h"
#include "property_widgets/SEPropertyVector2.h"
#include "component_widgets/SERootComponentWidget.h"
#include "MainWindow.h"
#include <QWidget>

SESystemLuaScript::SESystemLuaScript(SScene* scene) : SSceneSystemLuaScript(scene) {

}

SESystemLuaScript::~SESystemLuaScript() {

}

void SESystemLuaScript::initialize() {
    initializeLua();
    bindEditorTypes();

    StormFileSystem* fileSystem = StormEngine::getModule<StormFileSystem>();
    loadCommonScripts(fileSystem->getFilesList("lua_common/", "lua"));
    loadCommonScripts(fileSystem->getFilesList("lua_editor/common/", "lua"));

    initializeScene();


    for (SSceneComLuaScript* component : _ScriptComponents) {
        component->executeOnLoad();
    }
}

void SESystemLuaScript::bindEditorTypes() {
    getLua().new_usertype<QWidget>("QtWidget");
    getLua().new_usertype<SERootComponentWidget>("ERootComWidget");

    getLua().new_usertype<SEPropertyVector2>("EPropVec2",
        "setValue", &SEPropertyVector2::setValue,
        "getValue", &SEPropertyVector2::getValue
    );
    _LuaState["EPropVec2"]["new"] = &SEPropertyVector2::create;
}