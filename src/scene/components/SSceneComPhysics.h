#pragma once
#include "../SSceneComponent.h"
#include "../../core/utils/Geometry.h"

/* Linked objects can ONLY be static bodyes ! */

class b2Body;
class b2World;
class b2Fixture;

class SSceneComPhysics : public SSceneComponent {
public:
    SSceneComPhysics(StormSceneObject* owner);
    virtual ~SSceneComPhysics();

    virtual void serializeXml(pugi::xml_node& node);

    virtual int deserializeXml(pugi::xml_node& node);

    virtual int initialize(SSceneComponentSystem* system);

    /* Observer method. Called when transform component fires event.
     * Used to handle geometry transformations. */
    void observeTransformChanged(void* data);

    /* Synchronize transform position with physics body.
     * If body is static, transform position will always be forced on to body.
     * If body is dynamic / kinematic, transform position will be force set
     *    to body position, unless user manually changed transform position
     *    in which case body will take transform's position */
    void syncTransformAndPhysics();

    /* Returns true if collider contains @point */
    bool containsPoint(const Vector2& point);

    /* Returns Box2D body object */
    b2Body* getBox2DBody();

    /* Returns whether collider is used only as trigger. */
    bool isTrigger();

    /* Returns true if this body affects transform component */
    bool isDynamic();
    
    GeometryType getGeometryType();

    /* Binds this component to Scene Object's lua interface. */
    virtual void bindToScript(sol::state& luaState);

private:
    GeometryType _GeometryType;

    /* Size argument loaded from XML */
    Vector2 _GeometrySize;

    /* Pointer to Box2D physics body object */
    b2Body* _Box2DBody;

    /* Pointer to Box2D body's fixture object */
    b2Fixture* _Box2DFixture;

    /* Type of physics body */
    int _BodyType;

    /* Is this collider used only as trigger */
    bool _IsTrigger;

    /* Has transform been changed sence last tranfrom/physics synchronization */
    bool _IsTransformChanged;

    /* Generated new Box2D body based on component parameters,
     * and adds it to the world @world.
     * Returns false of failure */
    bool generateBox2DBody(b2World* world);

    void transformGeometryPlane();
}; 
