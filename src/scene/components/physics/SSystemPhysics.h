#pragma once
#include "scene/SSceneComponentSystem.h"
#include "utils/StormVertex.h"
#include "utils/math/ScalarMath.h"
#include "Box2D/Box2D.h"

/* System used for handling colliders */

class SComPhysics;
class Plane;
class b2ContactListener;

class SSystemPhysics : public SSceneComponentSystem {
public:
    SSystemPhysics(SScene* scene);
    virtual ~SSystemPhysics();

    virtual void initialize();

    /* Used for debug rendering */
    virtual void render(StormRenderer* renderer);

    virtual void tick(float deltaTime);

    virtual void initializeLua(sol::state& luaState);

    virtual void bindComponentsToLua(SSystemLuaScript* luaSystem);

    virtual void onComponentAdded(SSceneComponent* component);

    virtual void onComponentRemoved(SSceneComponent* component);

    b2World* getBox2DWorld();

private:
    /* Class used for listening for contact between objects in Box2D world */
    class ContactListener : public b2ContactListener {
    public:
        SSystemPhysics& _System;
        ContactListener(SSystemPhysics& system) : _System(system) { }
        void BeginContact(b2Contact* contact);
        void EndContact(b2Contact* contact);
    };
    /* Vector containing all components. Used for faster component access. */
    std::vector<SComPhysics*> _PhysicsComponents;

    /* Box2D world object. Holds and simulate rigid bodyes */
    b2World* _Box2DWorld;
    
    ContactListener _ContactListener;
};
 
