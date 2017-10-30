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
