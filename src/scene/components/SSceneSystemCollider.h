#pragma once
#include "../SSceneComponentSystem.h"

/* System used for handling colliders */

class SSceneComCollider;

class SSceneSystemCollider : public SSceneComponentSystem {
public:
    SSceneSystemCollider();
    virtual ~SSceneSystemCollider();

    virtual void addComponent(SSceneComponent* component);

    /* Used for debug rendering */
    virtual void render(StormRenderer* renderer);

    virtual int bindToLua(sol::state& luaState);

private:
    /* Vector containing all components. Used for faster component access. */
    std::vector<SSceneComCollider*> _ColliderComponents;

};
 
