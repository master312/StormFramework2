#pragma once
#include "../SSceneComponentSystem.h"

/* System used for rendering static textures */

class SSceneComStaticTexture;

class SSceneSystemStaticTexture : public SSceneComponentSystem {
public:
    SSceneSystemStaticTexture();
    virtual ~SSceneSystemStaticTexture();

    virtual void addComponent(SSceneComponent* component);

    virtual void render(StormRenderer* renderer);

private:
    /* Vector containing all components managed by this system */
    std::vector<SSceneComStaticTexture*> _Components;
};