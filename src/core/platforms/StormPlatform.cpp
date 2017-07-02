#include "StormPlatform.h"
#include "../StormCommon.h"

StormPlatform::StormPlatform() {
    _IsInitialized = false;
    _PlatformType = STORM_PLATFORM_NONE;
    _InputManager = nullptr;
    _ShouldQuit = false;
    _MainTickingFunction = nullptr;
    _WindowEventListener = nullptr;
}

StormPlatform::~StormPlatform() {
    if (_InputManager) {
        delete _InputManager;
    }
}

void StormPlatform::setMainTickingFunction(std::function<void(float)> tickingFun) {
    _MainTickingFunction = tickingFun;
}

void StormPlatform::setWindowEventListener(std::function<void(StormWindowEventType)> listener) {
    _WindowEventListener = listener;
}

void StormPlatform::startMainLoop() {
    if (!_MainTickingFunction) {
        LOG(FATAL) << "Main function not set!";
        return;
    }

    uint32_t startTime = 0;
    uint32_t timeDiff = 0;
    float deltaTime = 0.0f;
    float averageFps = 0.0f;
    int averageFpsCounter = 0;

    while (!_ShouldQuit) {
        startTime = getRunningTime();

        processEvents();
        _MainTickingFunction(deltaTime);

        timeDiff = getRunningTime() - startTime;
        deltaTime = (float)timeDiff / 1000.0f;
        
        if (deltaTime <= 0.0f) {
            /* Just makes sure we dont devide by zero */
            deltaTime = 0.0001f;
        }
        averageFps += 1000 / (1000 * deltaTime);
        averageFpsCounter++;
        if (averageFpsCounter >= 100) {
            LOG(INFO) << "FPS: " << (float)averageFps / (float)averageFpsCounter;
            averageFpsCounter = 0;
            averageFps = 0.0f;
        }
    }
}

bool StormPlatform::shouldQuit() {
    return _ShouldQuit;
}

void StormPlatform::quit() {
    _ShouldQuit = true;
}

StormPlatformType StormPlatform::getType() const {
    return _PlatformType;
}

StormWindowSettings StormPlatform::getWindowSettings() const {
    return _WindowSettings;
}

StormInputManager* StormPlatform::getInputManager() {
    return _InputManager;
}