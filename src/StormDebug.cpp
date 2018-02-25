#include "StormDebug.h"
#include "StormEngine.h"
#include "scene/SScene.h"
#include "scene/SSceneManager.h"

StormDebug::StormDebug() {
    _ShouldTickLogic = true;
    _SceneSavesCount = 0;
    _LastSavedSceneName = "";

    for (int i = 0; i < (int)S_SCENE_OBJECT_COM_TYPES_COUNT; i++) {
        _SystemsTickable[i] = true;
    }
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
        setSystemTickingEnabled(S_SCENE_OBJECT_COM_PHYSICS, !_SystemsTickable[S_SCENE_OBJECT_COM_PHYSICS]);
        LOG(INFO) << "Toggle physics ticking: " << _SystemsTickable[S_SCENE_OBJECT_COM_PHYSICS];
    } else if (StormEngine::getInputManager()->isKeyPressed(S_KEY_F9)) {
        saveCurrentScene();
    } else if (StormEngine::getInputManager()->isKeyPressed(S_KEY_F12)) {
        loadMostRecentScene();
    }
}

void StormDebug::setShouldTickLogic(bool value) {
    instance()->_ShouldTickLogic = value;
}

bool StormDebug::shouldTickLogic() {
    return instance()->_ShouldTickLogic;
}

void StormDebug::setSystemTickingEnabled(SSceneComponentType type, bool isEnabled) {
    instance()->_SystemsTickable[(int)type] = isEnabled;
}

bool StormDebug::shouldTickSystem(SSceneComponentType type) {
    return instance()->_SystemsTickable[(int)type];
}

void StormDebug::saveCurrentScene() {
    SScene* scene = StormEngine::getActiveScene();
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
    SScene* scene = manager->loadScene(_LastSavedSceneName, true);
    if (scene) {
        manager->switchScene(scene->getName());
    }
}
