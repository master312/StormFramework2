#include "StormDebug.h"
#include "StormEngine.h"
#include "scene/StormScene.h"
#include "scene/SSceneManager.h"

StormDebug::StormDebug() {
    _ShouldTickLogic = true;
    _ShouldTickPhysics = true;
    _SceneSavesCount = 0;
    _LastSavedSceneName = "";
}

StormDebug::~StormDebug() {
}

StormDebug* StormDebug::instance() {
    static StormDebug shared;
    return &shared;
}

void StormDebug::processInput() {
    if (StormEngine::getInputManager()->isKeyPressed(S_KEY_F2)) {
        _ShouldTickLogic = !_ShouldTickLogic;
        LOG(INFO) << "Toggle logic ticking: " << _ShouldTickLogic;
    } else if (StormEngine::getInputManager()->isKeyPressed(S_KEY_F4)) {
        _ShouldTickPhysics = !_ShouldTickPhysics;
        LOG(INFO) << "Toggle physics ticking: " << _ShouldTickPhysics;
    } else if (StormEngine::getInputManager()->isKeyPressed(S_KEY_F9)) {
        saveCurrentScene();
    } else if (StormEngine::getInputManager()->isKeyPressed(S_KEY_F12)) {
        loadMostRecentScene();
    }
}

bool StormDebug::shouldTickLogic() {
    return instance()->_ShouldTickLogic;
}

bool StormDebug::shouldTickPhysics() {
    return instance()->_ShouldTickPhysics;
}

void StormDebug::saveCurrentScene() {
    StormScene* scene = StormEngine::getActiveScene();
    if (scene) {
        _SceneSavesCount++;
        std::stringstream ss;
        std::string tmpSceneName = scene->getName();
        ss << "scenes/" << tmpSceneName << "_" <<_SceneSavesCount << ".xml";
        _LastSavedSceneName = ss.str();
        scene->saveXml(_LastSavedSceneName);
    }
    if (_SceneSavesCount > 4) {
        _SceneSavesCount = 0;
    }
}

void StormDebug::loadMostRecentScene() {
    if (_LastSavedSceneName == "") {
        return;
    }
    SSceneManager* manager = StormEngine::getModule<SSceneManager>();
    StormScene* scene = manager->loadScene(_LastSavedSceneName, true);
    if (scene) {
        manager->switchScene(scene->getName());
    }
}