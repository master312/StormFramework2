#include "SSceneSystemTransform.h"
#include "SSceneComTransform.h"

SSceneSystemTransform::SSceneSystemTransform() {
    _Type = S_SCENE_OBJECT_COM_SYS_TRANSFORM;
}

SSceneSystemTransform::~SSceneSystemTransform() {
}

void SSceneSystemTransform::tick(float deltaTime) {
    for (SSceneComTransform* component : _TransformComponents) {
        if (component->isChanged()) {
            component->transform();
        }
    }
}

void SSceneSystemTransform::addComponent(SSceneComponent* component) {
    if (!validateComponent(component, S_SCENE_OBJECT_COM_TRANSFORM)) {
        return;
    }
    SSceneComponentSystem::addComponent(component);

    SSceneComTransform* transform = dynamic_cast<SSceneComTransform*>(component);
    if (!transform) {
        LOG(ERROR) << "Could not cast component to SSceneComTransform";
        return;
    }

    _TransformComponents.push_back(transform);
}
