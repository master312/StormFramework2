#include "SSceneManager.h"
#include "StormEngine.h"
#include "graphics/StormRenderer.h"

SSceneManager::SSceneManager() : StormModuleBase("StormSceneManager") {
}

SSceneManager::~SSceneManager() {
}

SScene* SSceneManager::loadScene(const std::string& filename, bool reloadActive /* = false */) {
    spStormResourceFile sceneFile = StormEngine::getResource(filename);
    if (!sceneFile) {
        LOG(ERROR) << "Could not load scene file " << filename;
        return nullptr;
    }

    SScene* scene = new SScene();
    if (scene->loadXml(sceneFile) < 0) {
        /* Error occurred while loading scene */
        delete scene;
        return nullptr;
    }
    const std::string sceneName = scene->getName();

    LOG(DEBUG) << "Scene '" << sceneName << "' loaded.";

    auto iter = _LoadedScenes.find(sceneName);
    if (iter != _LoadedScenes.end()) {
        /* Scene with same name already exists in loaded scenes map */
        LOG(WARNING) << "Scene '" << sceneName << "' already loaded. Reloading... (TODO: NOT TESTED PROBABLY BROKEN)";
        if (_ActiveScene == iter->second && !reloadActive) {
            /* Scene is active. Can not reload scene while active */
            LOG(ERROR) << "Could not reload scene '" << sceneName << "'. Scene is currently active";
            delete scene;
            return nullptr;
        } else if (reloadActive) {
            _ActiveScene = nullptr;
            LOG(ERROR) << "TODO: Reloading of active scene. Not implemented yet...";
        }

        unloadScene(sceneName);

        _LoadedScenes[sceneName] = scene;
        return scene;
    }
    
    _LoadedScenes[sceneName] = scene;

    return scene;
}

SScene* SSceneManager::getActiveScene() {
    return _ActiveScene;
}

void SSceneManager::switchScene(const std::string& sceneName) {
    auto iter = _LoadedScenes.find(sceneName);
    if (iter == _LoadedScenes.end()) {
        /* Scene not found */
        LOG(ERROR) << "Tried to switch to non existing scene '" << sceneName << "'";
        return;
    } else if (!iter->second) {
        LOG(ERROR) << "Could not switch scene '" << sceneName << "' pointer is nullptr!";
        _LoadedScenes.erase(sceneName);
        return;
    }

    if (_ActiveScene == iter->second) {
        LOG(WARNING) << "Tried to switch to scene '" << sceneName << "' but scene is already active";
        return;
    }

    /* TODO: 
    if (_ActiveScene) 
        _ActiveScene->pause();
    */

    S_FIRE_GLOBAL_NOTIFICATION(SNotificationType::SCENE_MANAGER_SCENE_ABOUT_TO_CHANGE, iter->second);

    if (_ActiveScene) {
        /* TODO: Temporary fix...
         * Maybe dont unload active scene ? */
        const std::string& sceneName = _ActiveScene->getName();
        _ActiveScene = nullptr;
        unloadScene(sceneName);
    }

    _ActiveScene = iter->second;
    if (!_ActiveScene->isInitialized()) {
        _ActiveScene->initialize();
    }

    S_FIRE_GLOBAL_NOTIFICATION(SNotificationType::SCENE_MANAGER_SCENE_CHANGED, iter->second);
}

void SSceneManager::switchScene(const SScene* scene) {
    if (!scene) {
        return;
    }
    switchScene(scene->getName());
}

void SSceneManager::unloadScene(const std::string& name) {
    auto iter = _LoadedScenes.find(name);
    if (iter == _LoadedScenes.end()) {
        LOG(INFO) << "Tried to unload scene '" << name << "' but scene was not loaded";
        return;
    }

    if (iter->second) {
        delete iter->second;
    }

    if (iter->second == _ActiveScene) {
        LOG(ERROR) << "Tried to unload active scene '" << name << "'. This is not possible yet";
        return;
    }

    _LoadedScenes.erase(iter);
    LOG(DEBUG) << "Scene '" << name << "' unloaded";
}

void SSceneManager::unloadScene(const SScene* scene) {
    if (scene) {
        unloadScene(scene->getName());
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
