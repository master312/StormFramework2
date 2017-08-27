#include "StormEngine.h"
#include "core/StormCommon.h"
#include "core/platforms/StormPlatformSDL2.h"
#include "core/platforms/StormPlatformQt.h"
#include "core/graphics/StormVideoDriver.h"
#include "core/graphics/StormRenderer.h"
#include "core/graphics/StormShader.h"
#include "scene/SSceneManager.h"

StormEngine::StormEngine() {
    _IsInitialized = false;
    _Platform = nullptr;
    _ComVideoDriver = nullptr;
    _ComRenderer = nullptr;
    _ComTextureManager = nullptr;
    _GameDataFilesystem = nullptr;
    _DefaultShader = nullptr;
    _SceneManager = nullptr;

    _WindowInfo = StormWindowSettings(1280, 768, false, "The Storm Engine v 0.04", true);
}

StormEngine::~StormEngine() {
    deinitialize();
}

StormEngine* StormEngine::instance() {
    static StormEngine shared;
    return &shared;
}

void StormEngine::initialize(StormPlatformType platform) {
    switch(platform) {
        case STORM_PLATFORM_SDL2: {
#ifdef STORM_BUILD_PLATFORM_SDL2
            _Platform = new StormPlatformSDL2();
            if (_Platform->initialize() < 0) {
                LOG(FATAL) << "Could not initialize SDL platform.";
            }
#else
            LOG(FATAL) << "Tryed to initialzie SDL2 platform, but engine was built without SDL2 support.";
#endif
            break; }
        case STORM_PLATFORM_QT: {
#ifdef STORM_BUILD_PLATFORM_QT
            _Platform = new StormPlatformQt();
            if (_Platform->initialize() < 0) {
                LOG(FATAL) <<  "Could not initialize QT platform.";
            }
#else
            LOG(FATAL) << "Tryed to initialzie QT platform, but engine was built without QT support.";
#endif
            break; }
        default: {
            LOG(ERROR) << "Requested platform not implemented.";
            break; }
    }

    _GameDataFilesystem = new StormFileSystem("data/");

    if (_Platform->createWindow(_WindowInfo) < 0) {
        LOG(FATAL) << "Could not open window.";
    }

    if (initializeComponents() < 0) {
        LOG(FATAL) << "Could not initialize all engine components.";
    }

    _DefaultShader = new StormShader();
    if (_DefaultShader->compileFromSource(_GameDataFilesystem->getResourceByFilename("color.vs")->getBuffer(),
                                          _GameDataFilesystem->getResourceByFilename("color.fs")->getBuffer()) < 0) {
        LOG(FATAL) << "Default shader could not be compiled";
    }

    _DefaultShader->linkShaders();
    _ComRenderer->setShader(_DefaultShader);

    /* If this line is reached, all components have been initialized successfully */
    _Platform->setMainTickingFunction(std::bind(&StormEngine::mainTickingMethod, this, std::placeholders::_1));
    _Platform->setWindowEventListener(std::bind(&StormEngine::windowEventListener, this, std::placeholders::_1));

    /* Initialize scene manager, and load default scene */
    _SceneManager = new SSceneManager();
    _SceneManager->loadScene("scenes/the_scene.xml");
    _SceneManager->switchScene("the_scene");

    LOG(INFO) << "Engine components initialized successfully";
    _IsInitialized = true;
}

void StormEngine::deinitialize() {
    if (!_IsInitialized) {
        return;
    }
    _IsInitialized = false;

    if (_DefaultShader) {
        delete _DefaultShader;
        _DefaultShader = nullptr;
    }

    deinitializeComponents();

    if (_GameDataFilesystem) {
        delete _GameDataFilesystem;
        _GameDataFilesystem = nullptr;
    }
    if (_Platform) {
        delete _Platform;
        _Platform = nullptr;
    }

    LOG(INFO) << "Engine ended.";
}

int StormEngine::initializeComponents() {
    _ComVideoDriver = new StormVideoDriver();
    _ComVideoDriver->setVirtualViewSize(_WindowInfo.width, _WindowInfo.height);
    
    if (_ComVideoDriver->initialize(_WindowInfo.getSizePoint()) < 0) {
        return -1;
    }
    
    _Platform->getInputManager()->calculatePointerScaling(_ComVideoDriver->getVirtualViewSize());

    _ComRenderer = new StormRenderer();
    if (_ComRenderer->initialize() < 0) {
        return -1;
    }
    _ComRenderer->setPerspective(0.0f, 0.0f, _ComVideoDriver->getVirtualViewSize().x, _ComVideoDriver->getVirtualViewSize().y);


    _ComTextureManager = new StormTextureManager(_GameDataFilesystem);

    return 1;
}

void StormEngine::deinitializeComponents() {
    /* Make sure that order of exact opposite of initialization order */
    if (_ComTextureManager) {
        delete _ComTextureManager;
        _ComTextureManager = nullptr;
    }
    if (_ComRenderer) {
        delete _ComRenderer;
        _ComRenderer = nullptr;
    }
    if (_ComVideoDriver) {
        delete _ComVideoDriver;
        _ComVideoDriver = nullptr;
    }
}

void StormEngine::startMainLoop() {
    _Platform->startMainLoop();
}

void StormEngine::quit() {
    _Platform->quit();
}

StormRenderer* StormEngine::getRenderer() {
    return _ComRenderer;
}

StormPlatform* StormEngine::getPlatform() {
    return _Platform;
}

StormTextureManager* StormEngine::getTextureManager() {
    return _ComTextureManager;
}

StormFileSystem* StormEngine::getDataFilesystem() {
    return _GameDataFilesystem;
}

SSceneManager* StormEngine::getSceneManager() {
    return _SceneManager;
}

StormScene* StormEngine::getActiveScene() {
    return _SceneManager->getActiveScene();
}

void StormEngine::mainTickingMethod(float deltaTime) {
    updateTick(deltaTime);
    renderTick();
}

void StormEngine::windowEventListener(StormWindowEventType event) {
    switch (event) {
        case STORM_EVENT_WINDOW_RESIZED:
            _ComVideoDriver->setVirtualViewSize(_Platform->getWindowSettings().getSizeVec2());
            
            _Platform->getInputManager()->setRealWindowSize(_Platform->getWindowSettings().getSizeVec2());
            _Platform->getInputManager()->calculatePointerScaling(_ComVideoDriver->getVirtualViewSize());
            
            _ComRenderer->setPerspective(0.0f, 0.0f, _ComVideoDriver->getVirtualViewSize().x, _ComVideoDriver->getVirtualViewSize().y);
            LOG(DEBUG) << "Window resized to: " << _Platform->getWindowSettings().getSizeVec2();
            break;
    }
}

void StormEngine::renderTick() {
    _ComVideoDriver->begin();
    _ComVideoDriver->clear();
    _ComRenderer->startRendering();

    _SceneManager->render(_ComRenderer);

    _ComRenderer->endRendering();
    _Platform->windowSwapBuffers();
}

void StormEngine::updateTick(float deltaTime) {
    _SceneManager->tick(deltaTime);

    if (_Platform->getInputManager()->isKeyDown(S_KEY_ESCAPE)) {
        quit();
    }
}
