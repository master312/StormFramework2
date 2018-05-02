#pragma once
#include "core/resources/StormResourceFile.h"
#include "core/StormModuleBase.h"
#include "sol2/sol.hpp"

class SLuaSystem  : public StormModuleBase {
public:
    SLuaSystem();
    ~SLuaSystem();

    /* Initialize lua libraries and bind basic user types */
    void initialize(const std::string& rootLuaPath);

    /* Returns reference to lua state */
    sol::state& getState();

    /* Loads lua script file @luaFile and returns table
     * pointing to the return value of the script. */
    sol::table loadToTable(spStormResourceFile luaFile);

    /* Creates new LUA environment, and loads @luaFile into it. */
    sol::environment loadToEnv(spStormResourceFile luaFile);

private:
    /* Main lua state */
    sol::state _LuaState;

};