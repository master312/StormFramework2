#ifdef STORM_BUILD_PLATFORM_QT
#include "StormPlatformQt.h"
#include <QEvent>
#include <QTimer>
#include <QTime>
#include <iostream>

StormPlatformQt::StormPlatformQt() : StormPlatform() {
    _PlatformType = STORM_PLATFORM_QT;
    _LastTickTime = 0;
    _AverageFps = 0.0f;
    _AverageFpsCounter = 0;
    _Time = new QTime();
}

StormPlatformQt::~StormPlatformQt() {
    if (_Time) {
        delete _Time;
    }
}

int StormPlatformQt::initialize() {
    _Time->start();
    _InputManager = new StormInputManager();
    _IsInitialized = true;
    _LastTickTime = getTimeMs();
    LOG(INFO) << "QT platform initialized";
    return 1;
}

void StormPlatformQt::deinitialize() {
}

int StormPlatformQt::createWindow(StormWindowSettings settings) {
    _InputManager->setRealWindowSize(settings.getSizeVec2());
    return 1;
}

void StormPlatformQt::windowSwapBuffers() {
}

void StormPlatformQt::processEvents() {
}

uint32_t StormPlatformQt::getTimeMs() {
    return static_cast<uint32_t>(_Time->elapsed());
}

uint64_t StormPlatformQt::getTimeNs() {
    /* TODO: NOT IMPLEMENTED IN QT PLATFORM */
    return 0;
}

void StormPlatformQt::mainTick() {
    float deltaTime = (float)(getTimeMs() - _LastTickTime) / 1000.0f;

    _MainTickingFunction(deltaTime);

    _LastTickTime = getTimeMs();

    _AverageFps += 1000.0f / (1000.0f * deltaTime);
    _AverageFpsCounter++;
    if (_AverageFpsCounter >= 100) {
        LOG(INFO) << "FPS: " << _AverageFps / (float)_AverageFpsCounter;
        _AverageFpsCounter = 0;
        _AverageFps = 0.0f;
    }
}

void StormPlatformQt::handleWidgetResize(int width, int height) {
    _WindowSettings.width = width;
    _WindowSettings.height = height;
    if (_WindowEventListener) {
        _WindowEventListener(STORM_EVENT_WINDOW_RESIZED);
    }
}

#endif /* STORM_BUILD_PLATFORM_QT */
