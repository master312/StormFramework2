#pragma once
#include "scene/components/transform/SSystemTransform.h"

/* Class that extends Storm Scene Transform system (SSystemTransform)
 * Used in editor */

class SComTransform;

class SESystemTransform : public SSystemTransform  {
    friend class SSceneComponentSystem;

public:
    SESystemTransform(SScene* scene);
    virtual ~SESystemTransform();

    virtual void render(StormRenderer* renderer);

    virtual void tick(float deltaTime);
};

