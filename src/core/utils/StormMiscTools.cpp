#include <cstring>
#include <algorithm>
#include "StormMiscTools.h"

int StormMiscTools::hashString(const std::string& string) {
    return hashCharArray(string.c_str());
}

int StormMiscTools::hashCharArray(const char* string) {
    /* Taken from Squirrel code */
	int l = strlen(string);
    /* seed */
	unsigned int h = (unsigned int)l;
	/* if string is too long, don't hash all its chars */
    int step = (l >> 5) | 1;
	for (; l >= step; l -= step, string++) {
		h = h ^ ((h << 5) + (h >> 2) + (unsigned int)*string);
    }
	return h;
}

void StormMiscTools::trimSpacesBeginEnd(std::string& value) {
	/* Removes leading spaces */
	value.erase(value.begin(), std::find_if(value.begin(), value.end(), std::bind1st(std::not_equal_to<char>(), ' ')));
	/* Removes trailing spaces */
	value.erase(std::find_if(value.rbegin(), value.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), value.end());
}