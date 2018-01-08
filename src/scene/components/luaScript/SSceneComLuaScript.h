#pragma once
#include "../../SSceneComponent.h"
#include "sol2/sol.hpp"

/* This component holds lua script stuff */

class SSceneComLuaScript : public SSceneComponent {
public:
    SSceneComLuaScript(SSceneObject* owner);
    virtual ~SSceneComLuaScript();

    /* Saves component data to @node */
    virtual void serializeXml(pugi::xml_node& node);

    /* Loads component data from @node. 
     * Returns < 0 on error */
    virtual int deserializeXml(pugi::xml_node& node);

    /* Loads script file, bind all storm components to lua and 
     * add script object to lua handler. */ 
    virtual int initialize(SSceneComponentSystem* system);

    /* Returns lua function defined in this script object */
    sol::function getFunction(const std::string& name);

    /* Returns script filename */
    std::string& getFilename();

    /* Execute onLoad script function.
     * Should be called right after script has been loaded and initialized */
    void executeOnLoad();

    /* Execute onStart script function.
     * Should be called on just before first tick */
    void executeOnStart();

private:
    /* Lua script filename */
    std::string _Filename;

    /* Pointer to lua table containing owner's Scene Object */
    sol::table _LuaHandle;

    /* Returns lua handler for this script */
    sol::table& getLuaHandle();
};