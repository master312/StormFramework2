#include "sol2/sol.hpp"
#include "SSceneSystemPhysics.h"
#include "SSceneComPhysics.h"
#include "StormDebug.h"
#include "scene/SScene.h"
#include "scene/components/transform/SSceneComTransform.h"
#include "scene/components/luaScript/SSceneSystemLuaScript.h"
#include "graphics/StormRenderer.h"

SSceneSystemPhysics::SSceneSystemPhysics(SScene* scene) : SSceneComponentSystem(scene),
                                                              _ContactListener(*this) {
    _Type = S_SCENE_OBJECT_COM_PHYSICS;
    _Box2DWorld = nullptr;
}

SSceneSystemPhysics::~SSceneSystemPhysics() {
    if (_Box2DWorld) {
        delete _Box2DWorld;
        _Box2DWorld = nullptr;
    }
}

void SSceneSystemPhysics::addComponent(SSceneComponent* component) {
    SSceneComponentSystem::addComponent(component);

    SSceneComPhysics* physicsCom = dynamic_cast<SSceneComPhysics*>(component);
    _PhysicsComponents.push_back(physicsCom);
}

void SSceneSystemPhysics::initialize() {
    /* Define the gravity vector. */
    b2Vec2 gravity(0.0f, 7.81f);
    _Box2DWorld = new b2World(gravity);
    _Box2DWorld->SetContactListener(&_ContactListener);

    SSceneComponentSystem::initialize();
}

void SSceneSystemPhysics::render(StormRenderer* renderer) {
    for (SSceneComPhysics* com : _PhysicsComponents) {
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
        StormVertex vertices[4];
        SSceneComTransform* comTransform = com->getOwner()->getTransform();
        if (!comTransform) {
            return;
        }
        for (int i = 0; i < 4; i++) {
            vertices[i].position.x = com->getBox2DBody()->GetPosition().x;
            vertices[i].position.y = com->getBox2DBody()->GetPosition().y;
        }
        vertices[0].position.x -= 0.05f;
        vertices[0].position.y -= 0.05f;
        vertices[1].position.x += 0.05f;
        vertices[1].position.y -= 0.05f;
        vertices[2].position.x += 0.05f;
        vertices[2].position.y += 0.05f;
        vertices[3].position.x -= 0.05f;
        vertices[3].position.y += 0.05f;
    
        renderer->bindVertexData(vertices, 4);
        renderer->setColorAdd(Color(225, 25, 25));
        renderer->setLineWidth(3);
        renderer->draw();
    } 
}

void SSceneSystemPhysics::tick(float deltaTime) {
    /* TODO: Move to fixed tick */
#ifndef PRODUCTION
    if (StormDebug::shouldTickPhysics()) {
        _Box2DWorld->Step(deltaTime, 2, 6);
    }
#else
    _Box2DWorld->Step(deltaTime, 2, 6);
#endif

    for (size_t i = 0; i < _PhysicsComponents.size(); i++) {
        SSceneComPhysics* collider = _PhysicsComponents[i];
        collider->syncTransformAndPhysics();
    }
}

void SSceneSystemPhysics::initializeLua(sol::state& luaState) {
    luaState.new_usertype<SSceneComPhysics>("ComPhysics",
        "containsPoint", &SSceneComPhysics::containsPoint,
        "applyForce", &SSceneComPhysics::applyForce,
        "applyLinearImpulse", &SSceneComPhysics::applyLinearImpulse
    );
}

void SSceneSystemPhysics::bindComponentsToLua(SSceneSystemLuaScript* luaSystem) {
    for (SSceneComPhysics* com : _PhysicsComponents) {
        luaSystem->bindComponentToObject<SSceneComPhysics*>(com);
    }
}

b2World* SSceneSystemPhysics::getBox2DWorld() {
    return _Box2DWorld;
}

void SSceneSystemPhysics::ContactListener::BeginContact(b2Contact* contact) {
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

    SSceneComPhysics* component1 = static_cast<SSceneComPhysics*> (userData1);
    SSceneComPhysics* component2 = static_cast<SSceneComPhysics*> (userData2);
    component1->handleCollision(component2);
    component2->handleCollision(component1);
}

void SSceneSystemPhysics::ContactListener::EndContact(b2Contact* contact) {
}