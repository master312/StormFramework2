/* INFO: Every storm class will be depended on stuff included here */
#ifdef STORM_BUILD_PLATFORM_QT
/* On QT platform we must include GL headers before anything else. */
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif /* STORM_BUILD_PLATFORM_QT */

#include "../../libs/easyloggingpp/easylogging++.h"
#include "utils/StormMiscTools.h"
#include "utils/StormRefCounter.h"

#define S_ASSERT(condition) { if(!(condition)) { LOG(FATAL) << "Assertion failed at " << __FILE__ << ":" << __LINE__ << " inside " << __FUNCTION__ << " Condition: " << condition; } }

#ifdef PRODUCTION
    #define S_ASSERT_WARNING(condition) (condition)
#else
    #define S_ASSERT_WARNING(condition) { if(!(condition)) { LOG(WARNING) << "Assertion warning at " << __FILE__ << ":" << __LINE__ << " inside " << __FUNCTION__ << " Condition: " << condition; } }
#endif

#define makefourcc(a,b,c,d) ( ((unsigned int)a) | (((unsigned int)b)<< 8) | (((unsigned int)c)<<16) | (((unsigned int)d)<<24))