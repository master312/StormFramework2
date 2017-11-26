#pragma once
#include "../../SSceneComponentSystem.h"
#include "utils/StormVertex.h"
#include "utils/math/ScalarMath.h"
#include "Box2D/Box2D.h"

/* System used for handling colliders */

class SSceneComPhysics;
class Plane;
class b2ContactListener;

class SSceneSystemPhysics : public SSceneComponentSystem {
public:
    SSceneSystemPhysics();
    virtual ~SSceneSystemPhysics();

    virtual void addComponent(SSceneComponent* component);

    virtual void initialize(StormScene* ownerScene = nullptr);

    /* Used for debug rendering */
    virtual void render(StormRenderer* renderer);

    virtual void tick(float deltaTime);

    virtual int bindToLua(sol::state& luaState);

    b2World* getBox2DWorld();

private:
    /* Class used for listening for contact between objects in Box2D world */
    class ContactListener : public b2ContactListener {
    public:
        SSceneSystemPhysics& _System;
        ContactListener(SSceneSystemPhysics& system) : _System(system) { }
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);
    };
    /* Vector containing all components. Used for faster component access. */
    std::vector<SSceneComPhysics*> _PhysicsComponents;

    /* Box2D world object. Holds and simulate rigid bodyes */
    b2World* _Box2DWorld;
    
    ContactListener _ContactListener;
};
 
