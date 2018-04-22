#include "sol2/sol.hpp"
#include "SSystemPhysics.h"
#include "SComPhysics.h"
#include "StormDebug.h"
#include "scene/SScene.h"
#include "scene/components/transform/SComTransform.h"
#include "scene/components/luaScript/SSystemLuaScript.h"
#include "graphics/StormRenderer.h"

SSystemPhysics::SSystemPhysics(SScene* scene) : SSceneComponentSystem(scene),
                                                              _ContactListener(*this) {
    _Type = S_SCENE_OBJECT_COM_PHYSICS;
    _Box2DWorld = nullptr;

#ifndef PRODUCTION
    /* Force debugging system to ignore 'tick disabling' option
     * for this system. Debug tick disabling is handled
     * within this class. */
    _IgnoreDebugDisable = true;
#endif
}

SSystemPhysics::~SSystemPhysics() {
    if (_Box2DWorld) {
        delete _Box2DWorld;
        _Box2DWorld = nullptr;
    }
}

void SSystemPhysics::initialize() {
    /* Define the gravity vector. */
    b2Vec2 gravity(0.0f, 7.81f);
    _Box2DWorld = new b2World(gravity);
    _Box2DWorld->SetContactListener(&_ContactListener);

    SSceneComponentSystem::initialize();
}

void SSystemPhysics::render(StormRenderer* renderer) {
#ifndef PRODUCTION
    for (SComPhysics* com : _PhysicsComponents) {
    /* TODO: Rewrite this after primitive rendering system has been implemented */
        uint32_t indices[4] = {0, 1, 2, 3};
        //Vector2* vertices1 = com->getGeometry<Plane>()->getPoints();
        Vector2 vertices1[4];
        if (!com->getBox2DBody()) {
            continue;
        }
        b2Fixture* fixtures = com->getBox2DBody()->GetFixtureList();

        b2PolygonShape* shape = (b2PolygonShape*)fixtures[0].GetShape();
        for (int i = 0; i < 4; i++) {
            b2Vec2 tmpOne = shape->m_vertices[i];
            b2Vec2 tmp = com->getBox2DBody()->GetWorldPoint(tmpOne);
            vertices1[i].x = tmp.x;
            vertices1[i].y = tmp.y;
        }
        
        

        static StormVertex verticess[4];
        for (int i = 0; i < 4; i++) {
            verticess[i].position = vertices1[i];
        }

        /* Draw rect bounds */
        renderer->begin(S_RENDER_LINES_LOOP, true);
        renderer->setColorMultiply(Color(255, 255, 255, 255));
        renderer->setColorAdd(Color(255, 10, 10));
        renderer->bindVertexData(verticess, 4);
        renderer->bindIndexData(indices, 4);
        renderer->setLineWidth(1);
        renderer->draw();


        
        /* Draw rect center */
        SComTransform* comTransform = com->getOwner()->getTransform();
        if (!comTransform) {
            return;
        }

        RectF rect;
        rect.pos.x = com->getBox2DBody()->GetPosition().x - 0.05f;
        rect.pos.y = com->getBox2DBody()->GetPosition().y - 0.05f;
        rect.size.setXY(0.1f, 0.1f);

        renderer->drawRect(rect, 2.0f, Color(255, 0, 0, 0));
    }
#endif
}

void SSystemPhysics::tick(float deltaTime) {
    /* TODO: Move to fixed tick */
#ifndef PRODUCTION
    if (!_Box2DWorld) {
        return;
    }
    if (StormDebug::shouldTickSystem(getType())) {
        /* Tick physics only if ticking is not disabled in debug mode. */
        _Box2DWorld->Step(deltaTime, 2, 6);
    }
#else
    _Box2DWorld->Step(deltaTime, 2, 6);
#endif

    for (size_t i = 0; i < _PhysicsComponents.size(); i++) {
        SComPhysics* collider = _PhysicsComponents[i];
        collider->syncTransformAndPhysics();
    }
}

void SSystemPhysics::initializeLua(sol::state& luaState) {
    luaState.new_usertype<SComPhysics>("ComPhysics",
        "containsPoint", &SComPhysics::containsPoint,
        "applyForce", &SComPhysics::applyForce,
        "applyLinearImpulse", &SComPhysics::applyLinearImpulse
    );
}

void SSystemPhysics::bindComponentsToLua(SSystemLuaScript* luaSystem) {
    for (SComPhysics* com : _PhysicsComponents) {
        com->bindToLuaScript<SComPhysics*>();
    }
}

void SSystemPhysics::onComponentAdded(SSceneComponent* component) {
    if (!validateComponent(component, S_SCENE_OBJECT_COM_PHYSICS)) {
        return;
    }
    _PhysicsComponents.push_back(static_cast<SComPhysics*>(component));
}

void SSystemPhysics::onComponentRemoved(SSceneComponent* component) {
    for (size_t i = 0; i < _PhysicsComponents.size(); i++) {
        if (_PhysicsComponents[i] == component) {
            _PhysicsComponents.erase(_PhysicsComponents.begin() + i);
            return;
        }
    }
}

b2World* SSystemPhysics::getBox2DWorld() {
    return _Box2DWorld;
}

void SSystemPhysics::ContactListener::BeginContact(b2Contact* contact) {
    b2Fixture* fixture1 = contact->GetFixtureA();
    b2Fixture* fixture2 = contact->GetFixtureB();
    if (!fixture1 || !fixture2) {
        return;
    }
    if (!fixture1->IsSensor() && !fixture2->IsSensor()) {
        return;
    }

    b2Body* body1 = fixture1->GetBody(); 
    b2Body* body2 = fixture2->GetBody();
    void* userData1 = body1->GetUserData();
    void* userData2 = body2->GetUserData();
    if (!userData1 || !userData2) {
        LOG(ERROR) << "Box2D contact fiered with body without user data!";
        return;
    }

    SComPhysics* component1 = static_cast<SComPhysics*> (userData1);
    SComPhysics* component2 = static_cast<SComPhysics*> (userData2);
    component1->handleCollision(component2);
    component2->handleCollision(component1);
}

void SSystemPhysics::ContactListener::EndContact(b2Contact* contact) {
}