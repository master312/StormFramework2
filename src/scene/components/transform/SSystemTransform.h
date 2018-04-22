#pragma once
#include "scene/SSceneComponentSystem.h"

class SComTransform;

class SSystemTransform : public SSceneComponentSystem {
#ifdef STORM_EDITOR
    friend class SESystemTransform;
#endif

public:
    SSystemTransform(SScene* scene);
    virtual ~SSystemTransform();

    virtual void tick(float deltaTime);

    virtual void initializeLua(sol::state& luaState);

    virtual void bindComponentsToLua(SSystemLuaScript* luaSystem);

    virtual void onComponentAdded(SSceneComponent* component);

    virtual void onComponentRemoved(SSceneComponent* component);

private:
    std::vector<SComTransform*> _TransformComponents;

};