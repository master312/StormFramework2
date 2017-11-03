#include "SSceneSystemPlane.h"
#include "SSceneComPlane.h"
#include "../StormSceneObject.h"
#include "../../core/graphics/StormRenderer.h"
#include "SSceneComTransform.h"

SSceneSystemPlane::SSceneSystemPlane() {
    _Type = S_SCENE_OBJECT_COM_PLANE;
}

SSceneSystemPlane::~SSceneSystemPlane() {
}

void SSceneSystemPlane::addComponent(SSceneComponent* component) {
    if (!validateComponent(component, S_SCENE_OBJECT_COM_PLANE)) {
        return;
    }
    SSceneComponentSystem::addComponent(component);

    SSceneComPlane* plane = dynamic_cast<SSceneComPlane*>(component);
    if (!plane) {
        LOG(ERROR) << "Could not cast component to SSceneComPlane";
        return;
    }

    _PlaneComponents.push_back(plane);
}

void SSceneSystemPlane::render(StormRenderer* renderer) {
    for (SSceneComPlane* component : _PlaneComponents) {
        if (component->isRenderDebug()) {
            renderDebug(component, renderer);
        }
    }
}

int SSceneSystemPlane::bindToLua(sol::state& luaState) {
    luaState.new_usertype<SSceneComPlane>("ComPlane",
        "containsPoint", &SSceneComPlane::containsPoint
    );

    for (SSceneComPlane* com : _PlaneComponents) {
        sol::table handle = luaState["Handles"][com->getOwner()->getId()];
        if (!handle.valid() || !handle || !handle["isValid"]) {
            continue;
        }

        handle["obj"]["plane"] = com;
    }

    return 1;
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

    
    /* Draw rect center */
    StormVertex vertices[4];
    SSceneComTransform* comTransform = component->getOwner()->getTransform();
    if (!comTransform) {
        return;
    }
    for (int i = 0; i < 4; i++) {
        vertices[i].position = comTransform->getPositionAbs();
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