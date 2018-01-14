#pragma once
#include <iostream>

class StormMiscTools {
public:
    static int hashString(const std::string& string);
    static int hashCharArray(const char* string);
    /* Removes all free white spaces at the beginning and end of the string  */
    static void trimSpacesBeginEnd(std::string& value);
};