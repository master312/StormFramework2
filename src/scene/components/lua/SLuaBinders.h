#pragma once
#include "sol.hpp"

/* Class used to bind c++ stuff to lua */

class SLuaBinders {
public:
    SLuaBinders();
    ~SLuaBinders();

    /* Bind all script functions and classes to lua state @state 
     * Returns < 0 on error */
    static int bind(sol::state& state);


    /* Lua debug methods */
    static void debugLog(const std::string& msg);

private:
    static void bindVector2(sol::state& state);
    static void bindInputManager(sol::state& state);
};