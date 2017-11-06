#include "SSceneComPhysics.h"
#include "SSceneSystemPhysics.h"
#include "SSceneComTransform.h"
#include "Box2D/Box2D.h"
#include "../StormSceneObject.h"
#include "../../core/utils/Plane.h"
#include <algorithm>
#include <array>

/* Array of body type names, indexed by b2BodyType */
std::array<std::string, 4> bodyTypesString = {
    "static",
    "kinematic",
    "dynamic",
};


SSceneComPhysics::SSceneComPhysics(StormSceneObject* owner) : SSceneComponent(owner) {
    _Type = S_SCENE_OBJECT_COM_PHYSICS;
    _IsTrigger = false;

    _Box2DBody = nullptr;
    _Box2DFixture = nullptr;
    _BodyType = b2_staticBody;
    _GeometryType = GEOMETRY_TYPE_NONE;
    _IsTransformChanged = false;
    _ScriptHandlerName = "body";
}

SSceneComPhysics::~SSceneComPhysics() {
    if (_Box2DBody) {
        _Box2DBody->SetUserData(nullptr);
        _Box2DBody = nullptr;
    }
}

void SSceneComPhysics::serializeXml(pugi::xml_node& node) {
    SSceneComponent::serializeXml(node);
    
    switch (_GeometryType) {
        case GEOMETRY_TYPE_PLANE:
            node.append_attribute("geometry").set_value("plane");
            node.append_attribute("size_x").set_value(_GeometrySize.x);
            node.append_attribute("size_y").set_value(_GeometrySize.y);
            break;
        default:
            LOG(ERROR) << "Invalid shape while serializing ComPhysics";
            break;
    }

    if (_IsTrigger) {
        node.append_attribute("is_trigger").set_value(true);
    }
    
    const std::string& typeStr = bodyTypesString[_BodyType];
    node.append_attribute("body_type").set_value(typeStr.c_str());
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
        bool isFound = false;
        for (size_t i = 0; i < bodyTypesString.size(); i++) {
            const std::string& bodyStr = bodyTypesString[i];
            if (bodyStr == typeStr) {
                _BodyType = (int)i;
                isFound = true;
                break;
            }
        }
        if (!isFound) {
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
    SSceneComTransform* transform = _Owner->getTransform();
    if (transform) {
        Vector2 scale = transform->getScaleAbs();
        _GeometrySize.x *= scale.x;
        _GeometrySize.y *= scale.y;
    }

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
            if (!_IsTrigger && !_IsTransformChanged) {
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
    
    b2MassData mass;

    if (_IsTrigger) {
        mass.mass = 0.0f;
        mass.I = 0.0f;
        _Box2DBody->SetGravityScale(0.0f);
    } else {
        mass.mass = (rand() % 2) + 0.5f;
        mass.I = 1.5f;
    }
    
    mass.center = _Box2DBody->GetLocalCenter();
    _Box2DBody->SetMassData(&mass);

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

void SSceneComPhysics::applyForce(const Vector2& force) {
    if (!_Box2DBody) {
        return;
    }
    b2Vec2 b2Force(force.x, force.y);
    _Box2DBody->ApplyForceToCenter(b2Force, true);
}

void SSceneComPhysics::applyLinearImpulse(const Vector2& force) {
    if (!_Box2DBody) {
        return;
    }
    b2Vec2 b2Force(force.x, force.y);
    _Box2DBody->ApplyLinearImpulseToCenter(b2Force, true);
}

void SSceneComPhysics::bindToScript(sol::state& luaState) {
    bindTypeToScript<SSceneComPhysics*>(luaState);
}