#include <GL/glew.h>
#include "StormVideoDriver.h"
#include "../StormCommon.h"
#include "../utils/math/ScalarMath.h"

StormVideoDriver::StormVideoDriver() {
    _ClearColor = Color(0, 0, 200);
    _VirtualViewSize.set(0.0f, 0.0f);
}

StormVideoDriver::~StormVideoDriver() {
}

int StormVideoDriver::initialize(Point windowSize) {
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();
    if (glewError != GLEW_OK) {
        LOG(FATAL) << "Could not initialize glew. " << glewGetErrorString(glewError);
        return -1;
    }

    LOG(INFO) << "Glew initialized";
    return 1;
}

void StormVideoDriver::begin() {
}

void StormVideoDriver::clear() {
    glClearColor(_ClearColor.floatR(), _ClearColor.floatG(), _ClearColor.floatB(), 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void StormVideoDriver::setClearColor(Color color) {
    _ClearColor = color;
}

void StormVideoDriver::setVirtualViewSize(float width, float height) {
    _VirtualViewSize.set(width, height);
}

void StormVideoDriver::setVirtualViewSize(Vector2 size) {
    _VirtualViewSize = size;
}

Vector2 StormVideoDriver::getVirtualViewSize() const {
    return _VirtualViewSize;
}