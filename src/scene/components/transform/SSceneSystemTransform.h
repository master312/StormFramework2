#pragma once
#include "scene/SSceneComponentSystem.h"

class SSceneComTransform;

class SSceneSystemTransform : public SSceneComponentSystem {
#ifdef STORM_EDITOR
    friend class SESystemTransform;
#endif

public:
    SSceneSystemTransform(SScene* scene);
    virtual ~SSceneSystemTransform();

    virtual void tick(float deltaTime);

    virtual void initializeLua(sol::state& luaState);

    virtual void bindComponentsToLua(SSceneSystemLuaScript* luaSystem);

    virtual void onComponentAdded(SSceneComponent* component);

    virtual void onComponentRemoved(SSceneComponent* component);

private:
    std::vector<SSceneComTransform*> _TransformComponents;

};