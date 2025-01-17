#include "StormEngine.h"
#include "scene/SSceneManager.h"
#include "SEngineModuleFactory.h"
#include "StormDebug.h"
#include "lua/SLuaSystem.h"

#ifdef STORM_EDITOR
#include "StormEngineEditing.h"
#endif


const std::string StormEngine::DEFAULT_SHADER_NAME = "color";

StormEngine::StormEngine() {
    _ModPlatform = nullptr;
    _ModVideoDriver = nullptr;
    _ModRenderer = nullptr;
    _ModTextureManager = nullptr;
    _ModResources = nullptr;
    _ModSceneManager = nullptr;
    _ModLuaSystem = nullptr;
    _EventDispatcher = new SEventDispatcher::Dispatcher();

    _WindowInfo = StormWindowSettings(1280, 768, false, "The Storm Engine v 0.19", true);
}

StormEngine::~StormEngine() {
}

StormEngine* StormEngine::instance() {
#ifdef STORM_EDITOR
    static StormEngineEditing shared;
#else
    static StormEngine shared;
#endif
    return &shared;
}

void StormEngine::initialize(StormPlatformType platformType) {
    /* Initialize filesystem module */
    _ModResources = new StormFileSystem("data/");
    _ModulesByType[typeid(StormFileSystem)] = _ModResources->getModuleBase();

    /* Initialize platform module */
    _ModPlatform = SEngineModuleFactory::initPlatform(platformType);
    S_ASSERT(_ModPlatform);
    _ModulesByType[typeid(StormPlatform)] = _ModPlatform->getModuleBase();

    /* Initialize video driver module */
    _ModVideoDriver = SEngineModuleFactory::initVideoDriver();
    S_ASSERT(_ModVideoDriver);
    _ModulesByType[typeid(StormVideoDriver)] = _ModVideoDriver->getModuleBase();

    /* Initialize renderer module */
    _ModRenderer = SEngineModuleFactory::initRenderer(DEFAULT_SHADER_NAME);
    S_ASSERT(_ModRenderer);
    _ModulesByType[typeid(StormRenderer)] = _ModRenderer->getModuleBase();

    /* Initialize texture manager module */
    _ModTextureManager = new STextureManager();
    _ModulesByType[typeid(STextureManager)] = _ModTextureManager->getModuleBase();

    _ModLuaSystem = new SLuaSystem();
    _ModLuaSystem->initialize(_ModResources->getRootPath());
    _ModulesByType[typeid(_ModLuaSystem)] = _ModLuaSystem->getModuleBase();

    /* Initialize scene manager, and load test scene */
    _ModSceneManager = new SSceneManager();
    _ModSceneManager->loadScene("scenes/the_scene.xml");
    _ModSceneManager->switchScene("the_scene");
    _ModulesByType[typeid(SSceneManager)] = _ModSceneManager->getModuleBase();


    /* Bind ticking and event methods to platform module.
     * If this line is reached, all components have been initialized successfully */
    _ModPlatform->setMainTickingFunction(std::bind(&StormEngine::mainTickingMethod, this, std::placeholders::_1));
    _ModPlatform->setWindowEventListener(std::bind(&StormEngine::windowEventListener, this, std::placeholders::_1));

    LOG(INFO) << "Engine components initialized successfully";
}

void StormEngine::deinitialize() {
    for (auto& iter : _ModulesByType) {
        delete iter.second;
        iter.second = nullptr;
    }
    _ModulesByType.clear();
    LOG(INFO) << "All modules deleted successfully";
}

void StormEngine::startMainLoop() {
    if (!_ModPlatform) {
        LOG(FATAL) << "Can not start main loop without platform";
        return;
    }
    _ModPlatform->startMainLoop();
}

void StormEngine::quit() {
    _ModPlatform->quit();
}

void StormEngine::mainTickingMethod(float deltaTime) {
    updateTick(deltaTime);
    renderTick();
}

void StormEngine::windowEventListener(StormWindowEventType event) {
    switch (event) {
        case STORM_EVENT_WINDOW_RESIZED: {
            const Vector2 windowSize = _ModPlatform->getWindowSettings().getSizeVec2();

            _ModVideoDriver->setVirtualViewSize(windowSize);
            const Vector2 viewSize = _ModVideoDriver->getVirtualViewSize();

            _ModPlatform->getInputManager()->setRealWindowSize(windowSize);
            _ModPlatform->getInputManager()->calculatePointerScaling(viewSize);

            _ModRenderer->setViewMatrix(Vector2(0.0f, 0.0f), viewSize);
            LOG(DEBUG) << "Window resized to: " << windowSize;
            } break;
        case STORM_EVENT_WINDOW_SHOWN:
            break;
        case STORM_EVENT_WINDOW_HIDDEN:
            break;
        case STORM_EVENT_WINDOW_MOVED:
            break;
        case STORM_EVENT_WINDOW_FOCUS_GAINED:
            break;
        case STORM_EVENT_WINDOW_FOCUS_LOST:
            break;
    }
}

void StormEngine::renderTick() {
    _ModVideoDriver->begin();
    _ModVideoDriver->clear();
    _ModRenderer->startRendering();

    _ModSceneManager->render(_ModRenderer);

    _ModRenderer->endRendering();
    _ModPlatform->windowSwapBuffers();
}

void StormEngine::updateTick(float deltaTime) {
#ifndef PRODUCTION
    StormDebug::instance()->processInput();
    
    if (StormDebug::shouldTickLogic()) {
        _ModSceneManager->tick(deltaTime);
    }
#else
    _ModSceneManager->tick(deltaTime);
#endif
}

/**** Often used module methods for easy access ****/
spStormResourceFile StormEngine::getResource(const std::string& filename) {
    return instance()->_ModResources->getResourceByFilename(filename);
}

StormInputManager* StormEngine::getInputManager() {
    if (!instance()->_ModPlatform) {
        LOG(FATAL) << "Platform is nullptr!";
        return nullptr;
    }
    return instance()->_ModPlatform->getInputManager();
}

SSceneManager* StormEngine::getSceneManager() {
    if (!instance()->_ModSceneManager) {
        LOG(FATAL) << "Scene manager is nullptr!";
        return nullptr;
    }
    return instance()->_ModSceneManager;
}

SLuaSystem* StormEngine::getLua() {
    if (!instance()->_ModSceneManager) {
        LOG(FATAL) << "Lua system is nullptr!";
        return nullptr;
    }
    return instance()->_ModLuaSystem;
}

void StormEngine::fireEvent(SEventDispatcher::Event* event) {
    getEventDispatcher()->fireEvent(event);
}

SEventDispatcher::Dispatcher* StormEngine::getEventDispatcher() {
    if (!instance()->_EventDispatcher) {
        LOG(FATAL) << "Global event dispatcher is nullptr!";
        return nullptr;
    }
    return instance()->_EventDispatcher;
}

spStormTexture StormEngine::getTexture(const std::string& filename) {
    return instance()->_ModTextureManager->getTexture(filename);
}

SScene* StormEngine::getActiveScene() {
    return instance()->_ModSceneManager->getActiveScene();
}

uint32_t StormEngine::getTimeMs() {
    return instance()->_ModPlatform->getTimeMs();
}

uint64_t StormEngine::getTimeNs() {
    return instance()->_ModPlatform->getTimeNs();
}