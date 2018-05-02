#include "SLuaSystem.h"
#include "SLuaBindings.h"

SLuaSystem::SLuaSystem() : StormModuleBase("LuaSystem") {

}

SLuaSystem::~SLuaSystem() {
}

void SLuaSystem::initialize(const std::string& rootLuaPath) {
    _LuaState.open_libraries(sol::lib::base, sol::lib::os,
                             sol::lib::math, sol::lib::io,
                             sol::lib::count, sol::lib::package,
                             sol::lib::string, sol::lib::table,
                             sol::lib::debug);

    const std::string path = _LuaState["package"]["path"];
    _LuaState["package"]["path"] = path + ";" + rootLuaPath + "/?.lua";

    if (SLuaBindings::bindUserTypes(_LuaState) < 0) {
        LOG(ERROR) << "Could not bind lua user types";
    }
}

sol::state& SLuaSystem::getState() {
    return _LuaState;
}

sol::table SLuaSystem::loadToTable(spStormResourceFile luaFile) {
    if (!luaFile) {
        LOG(ERROR) << "Lua script can not be initialized. File not found.";
        return sol::table();
    }
    if (luaFile->getBufferSize() <= 4) {
        /* Script to short */
        LOG(ERROR) << "Invalid script file " << luaFile->getFilename();
        return sol::table();
    }
    return _LuaState.script(luaFile->getBuffer());
}

sol::environment SLuaSystem::loadToEnv(spStormResourceFile luaFile) {
    /* TODO ... */
}