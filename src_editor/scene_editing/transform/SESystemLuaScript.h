#pragma once
#include "scene/components/luaScript/SSceneSystemLuaScript.h"

class SSceneComLuaScript;

class SESystemLuaScript : public SSceneSystemLuaScript {
    friend class SSceneSystemLuaScript;

public:
    SESystemLuaScript(SScene* scene);
    virtual ~SESystemLuaScript();

    /* Load all scripts and execute initial methods */
    virtual void initialize();
};
