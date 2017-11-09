#include "SEngineModuleFactory.h"
#include "StormEngine.h"

/* TODO
    * Refactor all methods
    * Write some comments 
*/

StormPlatform* SEngineModuleFactory::initPlatform(StormPlatformType platformType) {
    StormPlatform* platform = nullptr;
    switch(platformType) {
        case STORM_PLATFORM_SDL2: {
#ifdef STORM_BUILD_PLATFORM_SDL2
            platform = dynamic_cast<StormPlatform*>(new StormPlatformSDL2());
            if (platform->initialize() < 0) {
                LOG(FATAL) << "Could not initialize SDL platform.";
                delete platform;
                platform = nullptr;
            }
#else
            LOG(FATAL) << "Tryed to initialzie SDL2 platform, but engine was built without SDL2 support.";
#endif
            break; }
        case STORM_PLATFORM_QT: {
#ifdef STORM_BUILD_PLATFORM_QT
            platform = dynamic_cast<StormPlatform*>(new StormPlatformQt());
            if (platform->initialize() < 0) {
                LOG(FATAL) <<  "Could not initialize QT platform.";
                delete platform;
                platform = nullptr;
            }
#else
            LOG(FATAL) << "Tryed to initialzie QT platform, but engine was built without QT support.";
#endif
            break; }
        default: {
            LOG(ERROR) << "Requested platform not implemented.";
            break; }
    }
    if (!platform) {
        LOG(FATAL) << "Could not initialize platform subsystem.";
        return nullptr;
    }

    if (platform->createWindow(StormEngine::instance()->_WindowInfo) < 0) {
        LOG(FATAL) << "Could not open window.";
        return nullptr;
    }

    return platform;
}

StormVideoDriver* SEngineModuleFactory::initVideoDriver() {
    StormVideoDriver* driver = new StormVideoDriver();
    StormWindowSettings& windowInfo = StormEngine::instance()->_WindowInfo;
    driver->setVirtualViewSize(windowInfo.width, windowInfo.height);
    if (driver->initialize(windowInfo.getSizePoint()) < 0) {
        LOG(FATAL) << "Could not initialize video driver.";
        return nullptr;
    }
    StormEngine::instance()->_ModPlatform->getInputManager()->calculatePointerScaling(driver->getVirtualViewSize());
    return driver;
}

StormRenderer* SEngineModuleFactory::initRenderer(const std::string& defaultShaderName) {
    StormRenderer* renderer = new StormRenderer();
    if (renderer->initialize() < 0) {
        LOG(FATAL) << "Could not initialize renderer.";
        return nullptr;
    }
    int shaderRes = renderer->loadShader(
            defaultShaderName, 
            StormEngine::getResource(defaultShaderName + ".vs")->getBuffer(),
            StormEngine::getResource(defaultShaderName + ".fs")->getBuffer()
        );
    if (shaderRes < 0) {
        LOG(FATAL) << "Could not load default shader";
        return nullptr;
    }
    renderer->setActiveShader(defaultShaderName);

    renderer->setViewMatrix(0.0f, 0.0f, StormEngine::instance()->_ModVideoDriver->getVirtualViewSize().x, StormEngine::instance()->_ModVideoDriver->getVirtualViewSize().y);

    return renderer;
}
