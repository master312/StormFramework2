#pragma once
#include "../SSceneComponent.h"
#include "sol.hpp"

/* This component holds lua script stuff */

class SSceneComLuaScript : public SSceneComponent {
public:
    SSceneComLuaScript(StormSceneObject* owner);
    virtual ~SSceneComLuaScript();

    /* Saves component data to @node */
    virtual void serializeXml(pugi::xml_node& node);

    /* Loads component data from @node. 
     * Returns < 0 on error */
    virtual int deserializeXml(pugi::xml_node& node);

    /* REMAINDER: Use this instend of SSceneComponent::initialize() !!!""
     * Loads script file, bind all components to lua and 
     * add script object to lua handler. */
    int initializeLua(sol::state& luaState);

    /* Returns lua handler for this script */
    sol::table& getLuaHandle();

    /* Returns script filename */
    std::string& getFilename();

private:
    /* Lua script filename */
    std::string _Filename;

    /* Pointer to script's lua table */
    sol::table _LuaHandler;
};