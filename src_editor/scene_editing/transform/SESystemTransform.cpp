#include "SESystemTransform.h"
#include "MainWindow.h"
#include "scene/SSceneObject.h"
#include "scene/components/transform/SComTransform.h"
#include "docks/object_hierarchy/SEDockObjectHierarchy.h"
#include "graphics/StormRenderer.h"

SESystemTransform::SESystemTransform(SScene* scene) : SSystemTransform(scene) {
}

SESystemTransform::~SESystemTransform() {
}

void SESystemTransform::render(StormRenderer* renderer) {
    SSceneObject* selectedObj = MainWindow::getHierarchyDock()->getSelectedObject();
    if (!selectedObj) {
        return;
    }
    SComTransform* transform = selectedObj->getTransform();
    if (!transform) {
        return;
    }

    Vector2 endPos = transform->getPositionAbs();
    float rotation = transform->getAngleAbs();
    static float length = 2.0f;
    endPos.x += StormScalarMath::cos((rotation * MATH_PI) / 180.0f) * length;
    endPos.y += StormScalarMath::sin((rotation * MATH_PI) / 180.0f) * length;

    renderer->drawLine(transform->getPositionAbs(), endPos, 5.0f, Color(10, 255, 10));

    endPos = transform->getPositionAbs();
    rotation = StormScalarMath::clampAngle(rotation - 90.0f);
    endPos.x += StormScalarMath::cos((rotation * MATH_PI) / 180.0f) * length;
    endPos.y += StormScalarMath::sin((rotation * MATH_PI) / 180.0f) * length;

    renderer->drawLine(transform->getPositionAbs(), endPos, 5.0f, Color(255, 255, 10));
}

void SESystemTransform::tick(float deltaTime) {
    SSystemTransform::tick(deltaTime);
}