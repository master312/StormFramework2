#include "StormInputManager.h"
#include "../StormCommon.h"

StormInputManager::StormInputManager() {
    resetAllEvents();
    _RealWindowSize.set(0.0f, 0.0f);
    _PointerScalingFactor.set(1.0f, 1.0f);
}

StormInputManager::~StormInputManager() {
}

void StormInputManager::begin() {
    memset(_KeysPressed, S_KEY_NONE, MAXIMUM_KEYS_PRESSED * sizeof(StormKey));
}

void StormInputManager::setRealWindowSize(Vector2 realWindowSize) {
    _RealWindowSize = realWindowSize;
}

void StormInputManager::calculatePointerScaling(Vector2 virtualViewSize) {
    _PointerScalingFactor.x = virtualViewSize.x / _RealWindowSize.x;
    _PointerScalingFactor.y = virtualViewSize.y / _RealWindowSize.y;
}

bool StormInputManager::isKeyDown(StormKey key) {
    for (int i = 0; i < MAXIMUM_KEYS_PRESSED; i++) {
        if (_KeysDown[i] == key) {
            return true;
        }
    }
    return false;
}

bool StormInputManager::isKeyPressed(StormKey key) {
    for (int i = 0; i < MAXIMUM_KEYS_PRESSED; i++) {
        if (_KeysPressed[i] == key) {
            return true;
        }
    }
    return false;
}

bool StormInputManager::isPointerDown(int pointerIndex /* = S_POINTER_LEFT */) {
    if (pointerIndex >= MAXIMUM_POINTERS_PRESSED) {
        LOG(WARNING) << "pointerIndex > then MAXIMUM_POINTERS_PRESSED";
        return false;
    }
    return _PointersDown[pointerIndex];
}

Vector2 StormInputManager::getPointerPosition(int pointerIndex /* = S_POINTER_LEFT */) {
    if (pointerIndex >= MAXIMUM_POINTERS_PRESSED) {
        LOG(WARNING) << "Button > then MAXIMUM_POINTERS_PRESSED";
        return Vector2(0.0f, 0.0f);
    }
    return _PointersPositions[pointerIndex];
}

void StormInputManager::resetAllEvents() {
    for (int i = 0; i < MAXIMUM_KEYS_PRESSED; i++) {
        _KeysDown[i] = S_KEY_NONE;
    }
    for (int i = 0; i < MAXIMUM_POINTERS_PRESSED; i++) {
        _PointersPositions[i].set(-1.0f, -1.0f);
        _PointersDown[i] = false;
    }
}

void StormInputManager::processKeyEvent(bool isDown, StormKey key) {
    for (int i = 0; i < MAXIMUM_KEYS_PRESSED; i++) {
        if (isDown) {
            if (_KeysDown[i] == S_KEY_NONE) {
                _KeysDown[i] = key;
                _KeysPressed[i] = key;
                return;
            }
        } else if (_KeysDown[i] == key) {
            _KeysDown[i] = S_KEY_NONE;
            return;
        }
    }

    if (isDown) {
        /* Maximum number of keys reached. Override the first one */
        _KeysDown[0] = key;
    }
}

void StormInputManager::processPointerMotion(int pointerIndex, int xPos, int yPos) {
    if (pointerIndex >= MAXIMUM_POINTERS_PRESSED) {
        return;
    }
    _PointersPositions[pointerIndex].set((float)xPos * _PointerScalingFactor.x, 
                                         (float)yPos * _PointerScalingFactor.y);
}

void StormInputManager::processPointerButton(int pointerIndex, bool isDown) {
    if (pointerIndex >= MAXIMUM_POINTERS_PRESSED) {
        return;
    }
    _PointersDown[pointerIndex] = isDown;
}
