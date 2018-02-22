#include "SESystemTransform.h"
#include "MainWindow.h"
#include "scene/SSceneObject.h"
#include "scene/components/transform/SSceneComTransform.h"
#include "docks/object_hierarchy/SEDockObjectHierarchy.h"
#include "graphics/StormRenderer.h"

SESystemTransform::SESystemTransform(SScene* scene) : SSceneSystemTransform(scene) {
}

SESystemTransform::~SESystemTransform() {
}

void SESystemTransform::render(StormRenderer* renderer) {
    SSceneObject* selectedObj = MainWindow::getHierarchyDock()->getSelectedObject();
    if (!selectedObj) {
        return;
    }
    SSceneComTransform* transform = selectedObj->getTransform();
    if (!transform) {
        return;
    }

    renderer->begin(S_RENDER_LINES_LOOP, true);
    renderer->setColorMultiply(Color(255, 255, 255, 255));
    renderer->setColorAdd(Color(10, 255, 10));

    Vector2 endPos = transform->getPositionAbs();
    float rotation = transform->getAngleAbs();
    static float length = 2.0f;
    endPos.x += StormScalarMath::cos((rotation * MATH_PI) / 180.0f) * length;
    endPos.y += StormScalarMath::sin((rotation * MATH_PI) / 180.0f) * length;

    renderer->prepareLineVertices(transform->getPositionAbs(), endPos);
    renderer->setLineWidth(5);
    renderer->draw();

    endPos = transform->getPositionAbs();
    rotation = StormScalarMath::clampAngle(rotation - 90.0f);
    endPos.x += StormScalarMath::cos((rotation * MATH_PI) / 180.0f) * length;
    endPos.y += StormScalarMath::sin((rotation * MATH_PI) / 180.0f) * length;

    renderer->setColorAdd(Color(255, 255, 10));
    renderer->prepareLineVertices(transform->getPositionAbs(), endPos);
    renderer->setLineWidth(5);
    renderer->draw();
}

void SESystemTransform::tick(float deltaTime) {
    SSceneSystemTransform::tick(deltaTime);
}