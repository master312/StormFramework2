#include "SSceneSystemPlane.h"
#include "SSceneComPlane.h"
#include "../StormSceneObject.h"
#include "../../core/graphics/StormRenderer.h"

SSceneSystemPlane::SSceneSystemPlane() {
    _Type = S_SCENE_OBJECT_COM_SYS_PLANE;
}

SSceneSystemPlane::~SSceneSystemPlane() {

}

void SSceneSystemPlane::addComponent(SSceneComponent* component) {
    if (!validateComponent(component, S_SCENE_OBJECT_COM_PLANE)) {
        return;
    }
    
    SSceneComPlane* plane = dynamic_cast<SSceneComPlane*>(component);
    if (!plane) {
        LOG(ERROR) << "Could not cast component to SSceneComPlane";
        return;
    }

    _Components.push_back(plane);
}

void SSceneSystemPlane::render(StormRenderer* renderer) {
    for (SSceneComPlane* component : _Components) {
        if (component->isRenderDebug()) {
            renderDebug(component, renderer);
        }
    }
}

void SSceneSystemPlane::renderDebug(SSceneComPlane* component, StormRenderer* renderer) {
    /* TODO: Rewrite this after primitive rendering system has been implemented */
    uint32_t indices[4] = {0, 1, 2, 3};

    /* Draw rect bounds */
    renderer->begin(S_RENDER_LINES_LOOP, true);
    renderer->setColorMultiply(Color(255, 255, 255, 255));
    renderer->setColorAdd(Color(255, 10, 10));
    renderer->bindVertexData(component->getVertices(), 4);
    renderer->bindIndexData(indices, 4);
    renderer->setLineWidth(1);
    renderer->draw();

    /* Draw rect pivot */
    StormVertex vertices[4];
    for (int i = 0; i < 4; i++) {
        vertices[i].position = component->getOwner()->getTransform().getPositionAbs();
    }
    vertices[0].position.x -= 4;
    vertices[0].position.y -= 4;
    vertices[1].position.x += 4;
    vertices[1].position.y -= 4;
    vertices[2].position.x += 4;
    vertices[2].position.y += 4;
    vertices[3].position.x -= 4;
    vertices[3].position.y += 4;

    renderer->bindVertexData(vertices, 4);
    renderer->setColorAdd(Color(225, 25, 25));
    renderer->setLineWidth(3);
    renderer->draw();
}