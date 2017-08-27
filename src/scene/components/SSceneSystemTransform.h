#pragma once
#include "../SSceneComponentSystem.h"

class SSceneComTransform;

class SSceneSystemTransform : public SSceneComponentSystem {
public:
    SSceneSystemTransform();
    virtual ~SSceneSystemTransform();

    virtual void tick(float deltaTime);

    virtual void addComponent(SSceneComponent* component);

private:
    std::vector<SSceneComTransform*> _TransformComponents;

};