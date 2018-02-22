#pragma once
#include "scene/components/transform/SSceneSystemTransform.h"

/* Class that extends Storm Scene Transform system (SSceneSystemTransform)
 * Used in editor */

class SSceneComTransform;

class SESystemTransform : public SSceneSystemTransform  {
    friend class SSceneComponentSystem;

public:
    SESystemTransform(SScene* scene);
    virtual ~SESystemTransform();

    virtual void render(StormRenderer* renderer);

    virtual void tick(float deltaTime);
};

