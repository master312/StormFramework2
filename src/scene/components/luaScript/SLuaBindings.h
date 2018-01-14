#pragma once
#include "sol2/sol.hpp"

/* Class used to bind c++ stuff to lua */

class SLuaBindings {
public:
    SLuaBindings();
    ~SLuaBindings();

    /* Bind all script functions and classes to lua state @state 
     * Returns < 0 on error */
    static int bindStandardTypes(sol::state& state);

    /* Binds SSceneObject usertype to lua state.
     * REMAINDER: Should be binded after standard types */
    static void bindSceneObject(sol::state& state);

    /* Lua debug methods */
    static void debugLog(const std::string& msg);

private:
    static void bindScene(sol::state& state);
    static void bindVector2(sol::state& state);
    static void bindInputManager(sol::state& state);
};
