#include "StormEngine.h"
#include "core/StormCommon.h"
#include "core/platforms/StormPlatformSDL2.h"
#include "core/platforms/StormPlatformQt.h"
#include "core/graphics/StormVideoDriver.h"
#include "core/graphics/StormRenderer.h"
#include "core/graphics/StormShader.h"
#include "core/resources/StormFileSystem.h"

StormEngine::StormEngine() {
    _IsInitialized = false;
    _Platform = nullptr;
    _ComVideoDriver = nullptr;
    _ComRenderer = nullptr;
    _ComTextureManager = nullptr;
    _GameDataFilesystem = nullptr;
    _DefaultShader = nullptr;
    
    _WindowInfo = StormWindowSettings(1280, 768, false, "The Storm Engine v 0.03", true);
    _ViewPerspectiveSize.set(1280, 768);
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
            _Platform = new StormPlatformSDL2();
            if (_Platform->initialize() < 0) {
                LOG(FATAL) << "Could not initialize SDL platform.";
            }
            break; }
        case STORM_PLATFORM_QT: {
            _Platform = new StormPlatformQt();
            if (_Platform->initialize() < 0) {
                LOG(FATAL) <<  "Could not initialize QT platform.";
            }
            break; }
        default: {
            LOG(ERROR) << "Requested platform not implemented.";
            break; }
    }

    _GameDataFilesystem = new StormFileSystem("../../data/");

    if (_Platform->createWindow(_WindowInfo) < 0) {
        LOG(FATAL) << "Could not open window.";
    }
    _Platform->getInputManager()->calculatePointerScaling(_ViewPerspectiveSize);

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
    if (_ComVideoDriver->initialize(_WindowInfo.getSize()) < 0) {
        return -1;
    }

    _ComRenderer = new StormRenderer();
    if (_ComRenderer->initialize() < 0) {
        return -1;
    }
    _ComRenderer->setPerspective(0.0f, 0.0f, _ViewPerspectiveSize.x, _ViewPerspectiveSize.y);

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

void StormEngine::mainTickingMethod(float deltaTime) {
    updateTick(deltaTime);
    renderTick();
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

#include "scene/StormScene.h"

StormScene* scene = nullptr;

void StormEngine::renderTick() {
    _ComVideoDriver->begin();
    _ComVideoDriver->clear();

    _ComRenderer->startRendering();

    if (!scene) {
        scene = new StormScene();
        scene->loadXml(_GameDataFilesystem->getResourceByFilename("scenes/the_scene.xml"));
        scene->initialize();
    }
    scene->render(_ComRenderer);

    _ComRenderer->endRendering();
    _Platform->windowSwapBuffers();
}

void StormEngine::updateTick(float deltaTime) {
    if (_Platform->getInputManager()->isKeyDown(S_KEY_ESCAPE)) {
        quit();
    }
}
