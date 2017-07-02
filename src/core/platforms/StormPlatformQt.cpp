#ifdef STORM_BUILD_PLATFORM_QT
#include "StormPlatformQt.h"
#include <QEvent>
#include <QTimer>
#include <QTime>
#include <iostream>

StormPlatformQt::StormPlatformQt() : StormPlatform() {
    _PlatformType = STORM_PLATFORM_QT;
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

uint32_t StormPlatformQt::getRunningTime() {
    return static_cast<uint32_t>(_Time->elapsed());
}

void StormPlatformQt::mainTick() {
    _MainTickingFunction(0.16);
}

void StormPlatformQt::handleWidgetResize(int width, int height) {
    _WindowSettings.width = width;
    _WindowSettings.height = height;
    if (_WindowEventListener) {
        _WindowEventListener(STORM_EVENT_WINDOW_RESIZED);
    }
}

#endif /* STORM_BUILD_PLATFORM_QT */
