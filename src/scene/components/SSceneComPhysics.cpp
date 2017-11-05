#include "SSceneComPhysics.h"
#include "SSceneSystemPhysics.h"
#include "SSceneComTransform.h"
#include "Box2D/Box2D.h"
#include "../StormSceneObject.h"
#include "../../core/utils/Plane.h"
#include <algorithm>

SSceneComPhysics::SSceneComPhysics(StormSceneObject* owner) : SSceneComponent(owner) {
    _Type = S_SCENE_OBJECT_COM_PHYSICS;
    _IsTrigger = false;

    _Box2DBody = nullptr;
    _Box2DFixture = nullptr;
    _BodyType = b2_staticBody;
    _GeometryType = GEOMETRY_TYPE_NONE;
    _IsTransformChanged = false;
}

SSceneComPhysics::~SSceneComPhysics() {
    if (_Box2DBody) {
        _Box2DBody->SetUserData(nullptr);
        _Box2DBody = nullptr;
    }
}

void SSceneComPhysics::serializeXml(pugi::xml_node& node) {
    SSceneComponent::serializeXml(node);
    
    // switch(_Geometry->getType()) {
    //     case GEOMETRY_TYPE_PLANE: {
    //         node.append_attribute("geometry").set_value("plane");
    //         Plane* tmpPlane = dynamic_cast<Plane*>(_Geometry);
    //         if (!tmpPlane) {
    //             LOG(ERROR) << "SSceneComPhysics serialize error 01";
    //             return;
    //         }

    //         node.append_attribute("size_x").set_value(tmpPlane->getSizeX());
    //         node.append_attribute("size_y").set_value(tmpPlane->getSizeY());
    //         break;
    //     } case GEOMETRY_TYPE_CIRCLE:
    //         node.append_attribute("geometry").set_value("circle");
    //         break;
    //     default:
    //         node.append_attribute("geometry").set_value("unknown");
    //         break;
    // }

    // node.append_attribute("is_trigger").set_value(_IsTrigger);
}

int SSceneComPhysics::deserializeXml(pugi::xml_node& node) {
    SSceneComponent::deserializeXml(node);
    
    std::string shapeName = node.attribute("geometry").as_string();
    std::transform(shapeName.begin(), shapeName.end(), shapeName.begin(), ::tolower);

    if (shapeName == "plane") {
        _GeometrySize.x = node.attribute("size_x").as_float(0.0f);
        _GeometrySize.y = node.attribute("size_y").as_float(0.0f);
        _GeometryType = GEOMETRY_TYPE_PLANE;
    } else {
        LOG(ERROR) << "Unsupported geometry type in physics component. OBJ ID: " << getOwner()->getId();
        return -1;
    }

    _IsTrigger = node.attribute("is_trigger").as_bool(false);
    if (!_IsTrigger) {
        std::string typeStr = node.attribute("body_type").as_string();
        if (typeStr == "kinematic") {
            _BodyType = b2_kinematicBody;
        } else if (typeStr == "dynamic") {
            _BodyType = b2_dynamicBody;
        } else if (typeStr == "static") {
            _BodyType = b2_staticBody;
        } else {
            LOG(ERROR) << "Invalid body type specified for physics component. OBJ ID: " << getOwner()->getId();
            _BodyType = b2_staticBody;
        }
    } else {
        /* This body is trigger. Triggers can only be static bodyes */
        _BodyType = b2_staticBody;
    }
    

    return 1;
}

int SSceneComPhysics::initialize(SSceneComponentSystem* system) {
    SSceneSystemPhysics* physicsSystem = dynamic_cast<SSceneSystemPhysics*>(system);
    SSceneComponent::initialize(system);
    
    if (_GeometryType == GEOMETRY_TYPE_NONE) {
        LOG(ERROR) << "Initializing physics component without geometry";
        return -1;
    }
    if (_Owner->getTransform()) {
        Vector2 scale = _Owner->getTransform()->getScaleAbs();
        _GeometrySize.x *= scale.x;
        _GeometrySize.y *= scale.y;
    }

    observeTransformChanged(nullptr);
    S_OBSERVER_ADD(_Owner, this, S_OBSERVER_EVENT_TRANSFORM_UPDATED, SSceneComPhysics::observeTransformChanged);
    
    if (!generateBox2DBody(physicsSystem->getBox2DWorld())) {
        LOG(ERROR) << "Physics component could not generate box2d body!";
        return -2;
    }
    
    return 1;
}

void SSceneComPhysics::observeTransformChanged(void* data) {
    if (_IsTransformChanged) {
        /* This will stay true only if transform was updated from 
         * SSceneComPhysics::syncTransformAndPhysics() method */
        return;
    }
    /* If this line is reached, that means position has been modifyed by external factors */
    SSceneComTransform* transform = _Owner->getTransform();
    
    if (_Box2DBody) {
        b2Vec2 position(transform->getAbsX(), transform->getAbsY());
        _Box2DBody->SetAwake(false);
        _Box2DBody->SetAwake(true);
        _Box2DBody->SetTransform(position, (transform->getAngleAbs() * b2_pi) / 180.0f);
    }

    _IsTransformChanged = true;
}

void SSceneComPhysics::syncTransformAndPhysics() {
    SSceneComTransform* transform = _Owner->getTransform();
    if (!transform || !_Box2DBody) {
        return;
    }

    switch (_BodyType) {
        case b2_staticBody: 
            {
            if (!_IsTransformChanged) {
                break;
            }
            /* If transform position changes, force set body's position to transfrom pos. */
            b2Vec2 position(transform->getAbsX(), transform->getAbsY());
            _Box2DBody->SetTransform(position, (transform->getAngleAbs() * b2_pi) / 180.0f);
            break; }
        case b2_dynamicBody:
        case b2_kinematicBody:
            {
            if (!_IsTrigger) {
                /* Body is not a trigger. Set transfrom to position of the body,
                 * And do nothing if body is trigger */
                const b2Vec2& position = _Box2DBody->GetPosition();
                transform->setPositionAbs(position.x, position.y);
                transform->setAngle(_Box2DBody->GetAngle() * (180.0 / b2_pi));
                _IsTransformChanged = true;
            }
            break; }
    }

    if (_IsTransformChanged) {
        transform->transform();
    }
    _IsTransformChanged = false;
}

bool SSceneComPhysics::containsPoint(const Vector2& point) {
    if (!_Box2DFixture) {
        return false;
    }
    b2Vec2 boxPoint(point.x, point.y);
    return _Box2DFixture->TestPoint(boxPoint);
}

bool SSceneComPhysics::generateBox2DBody(b2World* world) { 
    if (_Box2DBody) {
        LOG(WARNING) << "Tryed to generate multiple bodies for same physics component.";
        return false;
    }
    SSceneComTransform* transform = _Owner->getTransform();
    if (!transform) {
        LOG(ERROR) << "SSceneComPhysics::generateBox2DBody No transform component!";
        return false;
    }
    if (_Owner->getParent() && !_IsTrigger) {
        LOG(WARNING) << "SSceneComPhysics::generateBox2DBody Linked objects can ONLY be used as triggers.";
        LOG(WARNING) << "Making the body the trigger";
        _IsTrigger = true;
    }

    b2BodyDef bodyDeff;
    bodyDeff.type = (b2BodyType)_BodyType;
    bodyDeff.position.Set(transform->getAbsX(), transform->getAbsY());
    
    /* Generate actial body */
    _Box2DBody = world->CreateBody(&bodyDeff);

    b2PolygonShape shapePolygon;
    b2Vec2 shapeCenter(0.0f, 0.0f);
    switch (_GeometryType) {
        case GEOMETRY_TYPE_PLANE: {
            b2Vec2 size(_GeometrySize.x, _GeometrySize.y);
            /* Divide by 2 becouse Box2D expects half size */
            size /= 2.0f;
            float angle = (transform->getAngleAbs() * b2_pi) / 180.0f;
            shapePolygon.SetAsBox(size, shapeCenter, angle);
            break; }
        case GEOMETRY_TYPE_CIRCLE:
            /* TODO: */
            break;
    }

    b2FixtureDef fixture;
    fixture.isSensor = _IsTrigger;
    fixture.shape = &shapePolygon;
    _Box2DFixture = _Box2DBody->CreateFixture(&fixture);
    
    if (_IsTrigger) {
        b2MassData mass;
        mass.mass = 0.0f;
        mass.I = 0.0f;
        mass.center = _Box2DBody->GetLocalCenter();
        _Box2DBody->SetMassData(&mass);
        _Box2DBody->SetGravityScale(0.0f);
    }


    _Box2DBody->SetUserData((void*)this);

    return true;
}

b2Body* SSceneComPhysics::getBox2DBody() {
    return _Box2DBody;
}

bool SSceneComPhysics::isTrigger() {
    return _IsTrigger;
}

bool SSceneComPhysics::isDynamic() {
    if (!_Box2DBody) {
        return false;
    }
    /* Only static bodyes dose not affect transform */
    return _Box2DBody->GetType() != b2_staticBody && !_IsTrigger;
}

GeometryType SSceneComPhysics::getGeometryType() {
    return _GeometryType;
}