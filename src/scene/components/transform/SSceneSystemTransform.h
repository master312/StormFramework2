#pragma once
#include "../../SSceneComponentSystem.h"

class SSceneComTransform;

class SSceneSystemTransform : public SSceneComponentSystem {
public:
    SSceneSystemTransform(SScene* scene);
    virtual ~SSceneSystemTransform();

    virtual void tick(float deltaTime);

    virtual void addComponent(SSceneComponent* component);

    virtual int bindToLua(sol::state& luaState);

private:
    std::vector<SSceneComTransform*> _TransformComponents;

};