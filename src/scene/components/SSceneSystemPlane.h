#pragma once
#include "../SSceneComponentSystem.h"

/* System used for rendering planes in debug mode
 * And might be used for something more one day. */

class SSceneComPlane;

class SSceneSystemPlane : public SSceneComponentSystem {
public:
    SSceneSystemPlane();
    virtual ~SSceneSystemPlane();

    virtual void addComponent(SSceneComponent* component);

    virtual void render(StormRenderer* renderer);

private:
    /* Vector containing all components. Used for faster component access. */
    std::vector<SSceneComPlane*> _PlaneComponents;

    void renderDebug(SSceneComPlane* component, StormRenderer* renderer);
};
