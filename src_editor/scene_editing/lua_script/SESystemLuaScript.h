#pragma once
#include "scene/components/luaScript/SSceneSystemLuaScript.h"

class SSceneComLuaScript;
class SSceneObject;

class SESystemLuaScript : public SSceneSystemLuaScript {
    friend class SSceneSystemLuaScript;

public:
    SESystemLuaScript(SScene* scene);
    virtual ~SESystemLuaScript();

    /* Load all scripts and execute initial methods */
    virtual void initialize();

    /* Handles lua side of object selection changes.
     * Called when scene object gets selected in hierarchy dock*/
    void onObjectScelected(SSceneObject* object);

private:
    /* Load tools script */
    void loadToolScripts();

    void bindEditorTypes();
};
