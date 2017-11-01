#include "StormEngine.h"
#include "core/StormCommon.h"
#include "scene/SSceneManager.h"
#include "SEngineModuleFactory.h"

const std::string StormEngine::DEFAULT_SHADER_NAME = "color";

StormEngine::StormEngine() {
    _ModPlatform = nullptr;
    _ModVideoDriver = nullptr;
    _ModRenderer = nullptr;
    _ModTextureManager = nullptr;
    _ModResources = nullptr;
    _ModSceneManager = nullptr;

    _WindowInfo = StormWindowSettings(1280, 768, false, "The Storm Engine v 0.11", true);
}

StormEngine::~StormEngine() {
}

StormEngine* StormEngine::instance() {
    static StormEngine shared;
    return &shared;
}

void StormEngine::initialize(StormPlatformType platformType) {
    /* Initialize filesystem module */
    _ModResources = new StormFileSystem("data/");
    _ModulesByType[typeid(StormFileSystem)] = (void*)_ModResources;

    
    /* Initialize platform module */
    _ModPlatform = SEngineModuleFactory::initPlatform(platformType);
    if (!_ModPlatform) {
        return;
    }
    _ModulesByType[typeid(StormPlatform)] = (void*)_ModPlatform;


    /* Initialize video driver module */
    _ModVideoDriver = SEngineModuleFactory::initVideoDriver();
    if (!_ModVideoDriver) {
        return;
    }
    _ModulesByType[typeid(StormVideoDriver)] = (void*)_ModVideoDriver;


    /* Initialize renderer module */
    _ModRenderer = SEngineModuleFactory::initRenderer(DEFAULT_SHADER_NAME);
    if (!_ModRenderer) {
        return;
    }
    _ModulesByType[typeid(StormRenderer)] = (void*)_ModRenderer;
    

    /* Initialize texture manager module */
    _ModTextureManager = new StormTextureManager();
    _ModulesByType[typeid(StormTextureManager)] = (void*)_ModTextureManager;


    /* Initialize scene manager, and load test scene */
    _ModSceneManager = new SSceneManager();
    _ModSceneManager->loadScene("scenes/the_scene.xml");
    _ModSceneManager->switchScene("the_scene");
    _ModulesByType[typeid(SSceneManager)] = (void*)_ModSceneManager;


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
        case STORM_EVENT_WINDOW_RESIZED:
            _ModVideoDriver->setVirtualViewSize(_ModPlatform->getWindowSettings().getSizeVec2());
            
            _ModPlatform->getInputManager()->setRealWindowSize(_ModPlatform->getWindowSettings().getSizeVec2());
            _ModPlatform->getInputManager()->calculatePointerScaling(_ModVideoDriver->getVirtualViewSize());
            
            _ModRenderer->setPerspective(0.0f, 0.0f, _ModVideoDriver->getVirtualViewSize().x, _ModVideoDriver->getVirtualViewSize().y);
            LOG(DEBUG) << "Window resized to: " << _ModPlatform->getWindowSettings().getSizeVec2();
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
    _ModSceneManager->tick(deltaTime);
    
    if (_ModPlatform->getInputManager()->isKeyDown(S_KEY_ESCAPE)) {
        quit();
    }
}

/**** Commonly used module methods for easy access ****/
spStormResourceFile StormEngine::getResource(const std::string& filename) {
    return instance()->_ModResources->getResourceByFilename(filename);
}

StormInputManager* StormEngine::getInputManager() {
    if (!instance()->_ModPlatform) {
        return nullptr;
    }
    return instance()->_ModPlatform->getInputManager();
}

StormScene* StormEngine::getActiveScene() {
    return instance()->_ModSceneManager->getActiveScene();
}

uint32_t StormEngine::getTimeMs() {
    return instance()->_ModPlatform->getTimeMs();
}

uint64_t StormEngine::getTimeNs() {
    return instance()->_ModPlatform->getTimeNs();
}