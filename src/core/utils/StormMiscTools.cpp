#include <cstring>
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