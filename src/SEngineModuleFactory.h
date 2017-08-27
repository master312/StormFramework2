#pragma once
#include "core/platforms/StormPlatform.h"
#include "core/platforms/StormPlatformSDL2.h"
#include "core/platforms/StormPlatformQt.h"
#include "core/graphics/StormVideoDriver.h"
#include "core/graphics/StormRenderer.h"

/* This class contains initialization code for all modules.
 * WARNING: Engine instance MUST have filesystem already
 *          initialized before using any of the methods in this class*/


class SEngineModuleFactory {
    friend class StormEngine;
public:
    /* All method log fatal error and return nullptr on failure */

    /* Create and initialize platform module 
     * INITIALIZATION ORDER: 1 */
    static StormPlatform* initPlatform(StormPlatformType platformType);
    
    /* Create and intialize video driver module 
     * INITIALIZATION ORDER: 2 */
    static StormVideoDriver* initVideoDriver();

    /* Create and intialize video renderer module
     * INITIALIZATION ORDER: 3 */
    static StormRenderer* initRenderer(const std::string& defaultShaderName);

};
