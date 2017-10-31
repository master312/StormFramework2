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

    /* Loads script file, bind all storm components to lua and 
     * add script object to lua handler. */ 
    virtual int initialize(SSceneComponentSystem* system);

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