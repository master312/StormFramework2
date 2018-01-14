#pragma once
#include "../../SSceneComponentSystem.h"

class SSceneComTransform;

class SSceneSystemTransform : public SSceneComponentSystem {
public:
    SSceneSystemTransform(SScene* scene);
    virtual ~SSceneSystemTransform();

    virtual void tick(float deltaTime);

    virtual void addComponent(SSceneComponent* component);

    virtual void initializeLua(sol::state& luaState);

    virtual void bindComponentsToLua(SSceneSystemLuaScript* luaSystem);

    virtual void onComponentRemoved(SSceneComponent* component);

private:
    std::vector<SSceneComTransform*> _TransformComponents;

};