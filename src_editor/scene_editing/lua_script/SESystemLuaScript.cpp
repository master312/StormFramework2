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

    auto setValueFun = sol::resolve<void(const Vector2&)>(&SEPropertyVector2::setValue);
    getLua().new_usertype<SEPropertyVector2>("EPropVec2",
        "value", sol::property(&SEPropertyVector2::getValue, setValueFun),
        "setValue", &SEPropertyVector2::setValue,
        "getValue", &SEPropertyVector2::getValue,
        "onValueChanged", &SEPropertyVector2::setValueChangedLuaListener
    );
    _LuaState["EPropVec2"]["new"] = &SEPropertyVector2::create;
}