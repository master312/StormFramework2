#include "StormSceneElement.h"
#include "../core/graphics/StormRenderer.h"
#include "StormEngine.h"

StormSceneElement::StormSceneElement() {
    _Plane.setPosition(Vector2(100, 100));
    _Plane.setSize(Vector2(100, 100));
    _Plane.transform();
    _ZOrder = 0;
}

StormSceneElement::~StormSceneElement() {

}

void StormSceneElement::render() {
    StormVertex vertices[4];
    uint32_t indexData[] = { 0, 1, 2, 3 };

    for (int i = 0; i < 4; i++) {
        vertices[i].position.x = _Plane.getVertex(i)->x;
        vertices[i].position.y = _Plane.getVertex(i)->y;
    }

    if (StormEngine::instance()->getPlatform()->getInputManager()->isPointerDown()) {
        Vector2 pointerPos = StormEngine::instance()->getPlatform()->getInputManager()->getPointerPosition();
        for (int i = 0; i < 4; i++) {
            vertices[i].position += pointerPos;
        }
    }
    
    vertices[0].uv.set(0.0f, 0.0f);
    vertices[1].uv.set(1.0f, 0.0f);
    vertices[2].uv.set(1.0f, 1.0f);
    vertices[3].uv.set(0.0f, 1.0f);

    StormRenderer* renderer = StormEngine::instance()->getRenderer();
    
    StormRenderCommand* cmd = renderer->newCommand();
    cmd->setTexture(StormEngine::instance()->getTextureManager()->getTexture("icon.png").get());
    cmd->setVertexData(vertices, 4);
    cmd->setIndexData(indexData, 4);
    
    renderer->queueCommand(cmd);
}

void StormSceneElement::tick(float deltaTime) {

}

int StormSceneElement::setZ(int zOrder) {
    _ZOrder = zOrder;
}

int StormSceneElement::getZ() {
    return _ZOrder;
}

void StormSceneElement::setPosition(Vector2 position) {
    _Plane.setPosition(position);
    _Plane.transform();
}

void StormSceneElement::addPosition(Vector2 position) {
    _Plane.setPosition(_Plane.getPosition() + position);
    _Plane.transform();
}

Plane& StormSceneElement::getPlane() {
    return _Plane;
}