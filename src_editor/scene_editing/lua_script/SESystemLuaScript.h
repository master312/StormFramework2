#pragma once
#include "utils/SEventDispatcher.h"
#include "scene/components/luaScript/SSystemLuaScript.h"

class SComLuaScript;
class SSceneObject;

class SESystemLuaScript : public SSystemLuaScript {
    friend class SSystemLuaScript;

public:
    SESystemLuaScript(SScene* scene);
    virtual ~SESystemLuaScript();

    /* Load all scripts and execute initial methods */
    virtual void initialize();

    /* Listens for 'SNotificationType::EDITOR_SCENE_OBJECT_SELECTED' notification.
     * Handles lua side of object selection changes.
     * Called when scene object gets selected in hierarchy dock*/
    void sceneObjectSelected(const SEventDispatcher::Event* event);

    /* Sets whether game scripts should be ticked or not.
     * Dose not effect editor scripts. */
    void setTickGameScripts(bool value);

    /* Returns whether game scripts are ticking is enabled or not */
    bool getTickGameScripts();

    sol::function getGlobalFunction(const std::string& name);

private:
    bool _GameScriptsTicking;

    /* Load tools script */
    void loadToolScripts();

    void bindEditorTypes();
};
