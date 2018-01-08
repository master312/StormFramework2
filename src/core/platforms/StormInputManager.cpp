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
    int freeSpace = -1;
    for (int i = 0; i < MAXIMUM_KEYS_PRESSED; i++) {
        if (isDown) {
            if (freeSpace < 0 &&_KeysDown[i] == S_KEY_NONE) {
                freeSpace = i;
                break;
            }
            if (_KeysDown[i] == key) {
                return;
            }
        } else if (_KeysDown[i] == key) {
            _KeysDown[i] = S_KEY_NONE;
            return;
        }
    }

    if (freeSpace >= 0) {
        _KeysDown[freeSpace] = key;
        _KeysPressed[freeSpace] = key;
        return;
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


const char* StormInputManager::keyToString(const StormKey key) {
    switch (key) {
        case SDLK_0: return "KEY_0";
        case SDLK_1: return "KEY_1";
        case SDLK_2: return "KEY_2";
        case SDLK_3: return "KEY_3";
        case SDLK_4: return "KEY_4";
        case SDLK_5: return "KEY_5";
        case SDLK_6: return "KEY_6";
        case SDLK_7: return "KEY_7";
        case SDLK_8: return "KEY_8";
        case SDLK_9: return "KEY_9";
        case SDLK_a: return "A";
        case SDLK_QUOTE: return "'";
        case SDLK_b: return "B";
        case SDLK_BACKSLASH: return "\\\\";
        case SDLK_BACKSPACE: return "Backspace";
        case SDLK_c: return "C";
        case SDLK_CAPSLOCK: return "CapsLock";
        case SDLK_CLEAR: return "Clear";
        case SDLK_COMMA: return ",";
        case SDLK_COPY: return "Copy";
        case SDLK_CUT: return "Cut";
        case SDLK_d: return "D";
        case SDLK_DELETE: return "Delete";
        case SDLK_DOWN: return "Down";
        case SDLK_e: return "E";
        case SDLK_END: return "End";
        case SDLK_EQUALS: return "=";
        case SDLK_ESCAPE: return "Escape";
        case SDLK_f: return "F";
        case SDLK_F1: return "F1";
        case SDLK_F10: return "F10";
        case SDLK_F11: return "F11";
        case SDLK_F12: return "F12";
        case SDLK_F2: return "F2";
        case SDLK_F3: return "F3";
        case SDLK_F4: return "F4";
        case SDLK_F5: return "F5";
        case SDLK_F6: return "F6";
        case SDLK_F7: return "F7";
        case SDLK_F8: return "F8";
        case SDLK_F9: return "F9";
        case SDLK_FIND: return "Find";
        case SDLK_g: return "G";
        case SDLK_BACKQUOTE: return "`";
        case SDLK_h: return "H";
        case SDLK_HOME: return "Home";
        case SDLK_i: return "I";
        case SDLK_INSERT: return "Insert";
        case SDLK_j: return "J";
        case SDLK_k: return "K";
        case SDLK_l: return "L";
        case SDLK_LALT: return "Left_Alt";
        case SDLK_LCTRL: return "Left_Ctrl";
        case SDLK_LEFT: return "Left";
        case SDLK_LEFTBRACKET: return "[";
        case SDLK_LSHIFT: return "Left_Shift";
        case SDLK_m: return "M";
        case SDLK_MINUS: return "-";
        case SDLK_n: return "N";
        case SDLK_NUMLOCKCLEAR: return "Numlock";
        case SDLK_o: return "O";
        case SDLK_p: return "P";
        case SDLK_PAGEDOWN: return "PageDown";
        case SDLK_PAGEUP: return "PageUp";
        case SDLK_PASTE: return "Paste";
        case SDLK_PAUSE: return "Pause";
        case SDLK_PERIOD: return ".";
        case SDLK_PRINTSCREEN: return "PrintScreen";
        case SDLK_q: return "Q";
        case SDLK_r: return "R";
        case SDLK_RALT: return "Right_Alt";
        case SDLK_RCTRL: return "Right_Ctrl";
        case SDLK_RETURN: return "Return";
        case SDLK_RETURN2: return "Return";
        case SDLK_RIGHT: return "Right";
        case SDLK_RIGHTBRACKET: return "]";
        case SDLK_RSHIFT: return "Right_Shift";
        case SDLK_s: return "S";
        case SDLK_SCROLLLOCK: return "ScrollLock";
        case SDLK_SEMICOLON: return ";";
        case SDLK_SLASH: return "/";
        case SDLK_SPACE: return "Space";
        case SDLK_t: return "T";
        case SDLK_TAB: return "Tab";
        case SDLK_u: return "U";
        case SDLK_UNDO: return "Undo";
        case SDLK_UNKNOWN: return "";
        case SDLK_UP: return "Up";
        case SDLK_v: return "V";
        case SDLK_w: return "W";
        case SDLK_x: return "X";
        case SDLK_y: return "Y";
        case SDLK_z: return "Z";
        default:
            return nullptr;
    }
    return nullptr;
}