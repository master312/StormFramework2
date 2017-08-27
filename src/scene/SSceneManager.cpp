#include "SSceneManager.h"
#include "../StormEngine.h"
#include "../core/graphics/StormRenderer.h"

SSceneManager::SSceneManager() {
}

SSceneManager::~SSceneManager() {
}

int SSceneManager::loadScene(const std::string& filename) {
    StormFileSystem* fileSystem = StormEngine::instance()->getDataFilesystem();

    spStormResourceFile sceneFile = fileSystem->getResourceByFilename(filename);
    if (!sceneFile) {
        LOG(ERROR) << "Could not load scene file " << filename;
        return -1;
    }

    StormScene* scene = new StormScene();
    if (scene->loadXml(sceneFile) < 0) {
        /* Error occured while loading scene */
        delete scene;
        return -2;
    }
    const std::string sceneName = scene->getName();

    LOG(DEBUG) << "Scene '" << sceneName << "' loaded.";

    auto iter = _LoadedScenes.find(sceneName);
    if (iter != _LoadedScenes.end()) {
        /* Scene with same name already exists in loaded scenes map */
        LOG(WARNING) << "Scene '" << sceneName << "' already laoded. Reloading...";
        if (_ActiveScene == iter->second) {
            /* Scene is active. Can not reload scene while active */
            LOG(ERROR) << "Could not reload scene '" << sceneName << "'. Scene is currently active";
            delete scene;
            return -1;
        }
        delete iter->second;
        iter->second = nullptr;
        _LoadedScenes[sceneName] = scene;
        return 2;
    }
    
    _LoadedScenes[sceneName] = scene;

    return 1;
}

StormScene* SSceneManager::getActiveScene() {
    return _ActiveScene;
}

void SSceneManager::switchScene(const std::string& sceneName) {
    auto iter = _LoadedScenes.find(sceneName);
    if (iter == _LoadedScenes.end()) {
        /* Scene not found */
        LOG(ERROR) << "Tryed to switch to non existing scene '" << sceneName << "'";
        return;
    }
    if (_ActiveScene == iter->second) {
        LOG(WARNING) << "Tryed to switch to scene '" << sceneName << "' but scene is already active";
        return;
    }

    /* TODO: 
    if (_ActiveScene) 
        _ActiveScene->pause();
    */

    _ActiveScene = iter->second;
    if (!_ActiveScene->isInitialized()) {
        _ActiveScene->initialize();
    }
}

void SSceneManager::render(StormRenderer* renderer) {
    if (!_ActiveScene) {
        LOG(WARNING) << "No active scene specified";
        return;
    }
    
    _ActiveScene->render(renderer);
}

void SSceneManager::tick(float deltaTime) {
    if (!_ActiveScene) {
        LOG(WARNING) << "No active scene specified";
        return;
    }
    _ActiveScene->tick(deltaTime);

}
