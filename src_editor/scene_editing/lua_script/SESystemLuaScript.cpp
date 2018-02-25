#include "SESystemLuaScript.h"
#include "StormEngine.h"
#include "scene/SSceneObject.h"
#include "scene/components/luaScript/SSceneComLuaScript.h"
#include "property_widgets/SEPropertyVector2.h"
#include "property_widgets/SEPropertyNumber.h"
#include "component_widgets/SERootComponentWidget.h"

SESystemLuaScript::SESystemLuaScript(SScene* scene) : SSceneSystemLuaScript(scene) {

}

SESystemLuaScript::~SESystemLuaScript() {

}

void SESystemLuaScript::initialize() {
    initializeLua();
    bindEditorTypes();

    /* Sets lua "IsEditor" variable, so that script can know its beeing run from editor */
    _LuaState["IsEditor"] = true;

    StormFileSystem* fileSystem = StormEngine::getModule<StormFileSystem>();
    loadCommonScripts(fileSystem->getFilesList("lua_common/", "lua"));
    loadCommonScripts(fileSystem->getFilesList("lua_editor/common/", "lua"));

    initializeScene();

    for (SSceneComLuaScript* component : _ScriptComponents) {
        component->executeOnLoad();
    }

    loadToolScripts();
}

void SESystemLuaScript::onObjectScelected(SSceneObject* object) {
    sol::function objectSelectedFun = _LuaState["sceneObjectSelected"];
    if (!objectSelectedFun.valid()) {
        LOG(ERROR) << "Could not find lua function 'sceneObjectSelected'";
        return;
    }

    objectSelectedFun(object->getLuaHandle());
}

void SESystemLuaScript::bindEditorTypes() {
    _LuaState.new_usertype<QWidget>("QtWidget");
    _LuaState.new_usertype<SERootComponentWidget>("ERootComWidget",
        "setName", &SERootComponentWidget::setName,
        "isCollapsed", sol::property(&SERootComponentWidget::isCollapsed)
    );


    /* Bind SEPropertyVector2 to LUA */
    auto setValueFun = sol::resolve<void(const Vector2&)>(&SEPropertyVector2::setValue);
    _LuaState.new_usertype<SEPropertyVector2>("EPropVec2",
        "value", sol::property(&SEPropertyVector2::getValue, setValueFun),
        "setValue", &SEPropertyVector2::setValue,
        "getValue", &SEPropertyVector2::getValue,
        "onValueChanged", &SEPropertyWidget::setValueChangedLuaListener
    );
    _LuaState["EPropVec2"]["new"] = &SEPropertyVector2::create;


    /* Bind SEPropertyNumber to LUA */
    _LuaState.new_usertype<SEPropertyNumber>("EPropNumber",
        "value", sol::property(&SEPropertyNumber::getValue, &SEPropertyNumber::setValue),
        "onValueChanged", &SEPropertyWidget::setValueChangedLuaListener
    );
    _LuaState["EPropNumber"]["new"] = &SEPropertyNumber::create;
}

void SESystemLuaScript::loadToolScripts() {
    StormFileSystem* fileSystem = StormEngine::getModule<StormFileSystem>();
    const std::vector<std::string> files = fileSystem->getFilesList("lua_editor/tools/", "lua");

    sol::function toolCreateFun = _LuaState["createToolHandle"];
    if (!toolCreateFun.valid()) {
        LOG(ERROR) << "Lua function 'createToolHandle' not found!";
        return;
    }

    for (const std::string& filename : files) {
        sol::table table = loadScriptFile(filename);
        if (!table.valid()) {
            LOG(ERROR) << "Could not load tool script " << filename;
            continue;
        }

        bool result = toolCreateFun(table);
        if (!result) {
            LOG(ERROR) << "Invalid editor tool script " << filename;
        }
    }
}