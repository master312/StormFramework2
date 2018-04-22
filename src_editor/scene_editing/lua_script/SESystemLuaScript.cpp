#include "SESystemLuaScript.h"
#include "StormEngine.h"
#include "scene/SSceneObject.h"
#include "scene/components/luaScript/SComLuaScript.h"
#include "property_widgets/SEPropertyVector2.h"
#include "property_widgets/SEPropertyNumber.h"
#include "component_widgets/SERootComponentWidget.h"
#include <QApplication>

SESystemLuaScript::SESystemLuaScript(SScene* scene) : SSystemLuaScript(scene) {
    _GameScriptsTicking = false;
}

SESystemLuaScript::~SESystemLuaScript() {
    StormEngine::getEventDispatcher()->removeListeners<SESystemLuaScript>(this);
}

void SESystemLuaScript::initialize() {
    initializeLua();
    bindEditorTypes();

    /* Sets lua "IsEditor" variable, so that script can know its being run from editor */
    _LuaState["IsEditor"] = true;

    /* Dont tick game scripts by default */
    setTickGameScripts(false);

    StormFileSystem* fileSystem = StormEngine::getModule<StormFileSystem>();
    loadCommonScripts(fileSystem->getFilesList("lua_common/", "lua"));
    loadCommonScripts(fileSystem->getFilesList("lua_editor/common/", "lua"));

    initializeScene();

    for (SComLuaScript* component : _ScriptComponents) {
        component->executeOnLoad();
    }

    loadToolScripts();

    StormEngine::getEventDispatcher()->registerEventListener<SESystemLuaScript>(
            SEventDispatcher::SSceneObjectEvent::EDIT_OBJECT_SELECTED,
            &SESystemLuaScript::sceneObjectSelected, this);
}

void SESystemLuaScript::sceneObjectSelected(const SEventDispatcher::Event* event) {
    sol::function objectSelectedFun = getGlobalFunction("sceneObjectSelected");
    if (!objectSelectedFun.valid()) {
        LOG(ERROR) << "Could not find lua function 'sceneObjectSelected'";
        return;
    }
    const SEventDispatcher::SSceneObjectEvent* soEvent = static_cast<const SEventDispatcher::SSceneObjectEvent*>(event);

    if (soEvent && soEvent->object) {
        /* Object selected */
        objectSelectedFun(soEvent->object->getLuaHandle());
    } else {
        /* Object deselected */
        objectSelectedFun(nullptr);
    }
}

void SESystemLuaScript::setTickGameScripts(bool value) {
    _GameScriptsTicking = value;
    _LuaState["TickGameScripts"] = value;
}

bool SESystemLuaScript::getTickGameScripts() {
    return _GameScriptsTicking;
}

sol::function SESystemLuaScript::getGlobalFunction(const std::string& name) {
    return _LuaState[name.c_str()];
}

/* TODO: Temporary... Fix when sperated lua bindings class is created */
static void changeCursor(int cursor) {
    QApplication::setOverrideCursor((Qt::CursorShape)cursor);
}


void SESystemLuaScript::bindEditorTypes() {
    /* TODO: Move this function to separated class, like in engine */
    _LuaState.new_usertype<QWidget>("QtWidget");
    _LuaState.new_usertype<SERootComponentWidget>("ERootComWidget",
        "setName", &SERootComponentWidget::setName,
        "isCollapsed", sol::property(&SERootComponentWidget::isCollapsed)
    );


    _LuaState.new_usertype<QCursor>("QtCursor");
    _LuaState.new_usertype<QApplication>("QtApplication");
    _LuaState["QtApplication"]["setOverrideCursor"] = &changeCursor;
    _LuaState["QtApplication"]["SizeVerCursor"] = (int)Qt::SizeVerCursor;
    _LuaState["QtApplication"]["SizeHorCursor"] = (int)Qt::SizeHorCursor;
    _LuaState["QtApplication"]["SizeBDiagCursor"] = (int)Qt::SizeBDiagCursor;
    _LuaState["QtApplication"]["SizeFDiagCursor"] = (int)Qt::SizeFDiagCursor;
    _LuaState["QtApplication"]["SizeAllCursor"] = (int)Qt::SizeAllCursor;
    _LuaState["QtApplication"]["ArrowCursor"] = (int)Qt::ArrowCursor;

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

    sol::function toolCreateFun = getGlobalFunction("createToolHandle");
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