#include "SSceneComCollider.h"
#include "SSceneSystemCollider.h"
#include "SSceneComTransform.h"
#include "../StormSceneObject.h"
#include "../../core/utils/Plane.h"
#include <algorithm>

SSceneComCollider::SSceneComCollider(StormSceneObject* owner) : SSceneComponent(owner) {
    _Type = S_SCENE_OBJECT_COM_COLLIDER;
    
    _Geometry = nullptr;
}

SSceneComCollider::~SSceneComCollider() {

}

void SSceneComCollider::serializeXml(pugi::xml_node& node) {
    SSceneComponent::serializeXml(node);
}

int SSceneComCollider::deserializeXml(pugi::xml_node& node) {
    SSceneComponent::deserializeXml(node);
    
    std::string shapeName = node.attribute("geometry").as_string();
    std::transform(shapeName.begin(), shapeName.end(), shapeName.begin(), ::tolower);

    if (shapeName == "plane") {
        Plane* tmpPlane = new Plane();
        _PlaneSize.x = node.attribute("size_x").as_float(0.0f);
        _PlaneSize.y = node.attribute("size_y").as_float(0.0f);
        
        tmpPlane->setSize(_PlaneSize);

        _Geometry = tmpPlane;
    } else {
        LOG(ERROR) << "Unsupported geometry type in collider component. OBJ ID: " << getOwner()->getId();
        return -1;
    }

    return 1;
}

int SSceneComCollider::initialize(SSceneComponentSystem* system) {
    SSceneComponent::initialize(system);
    
    if (!_Geometry) {
        LOG(ERROR) << "Initializing collider component without geometry";
        return -1;
    }

    observeTransformChanged(nullptr);
    S_OBSERVER_ADD(_Owner, this, S_OBSERVER_EVENT_TRANSFORM_UPDATED, SSceneComCollider::observeTransformChanged);
    
    return 1;
}

void SSceneComCollider::observeTransformChanged(void* data) {
    if (!_Geometry) {
        return;
    }

    switch (_Geometry->getType()) {
        case GEOMETRY_TYPE_PLANE:
            transformGeometryPlane();
            break;
        default:
            LOG(ERROR) << "Collider component has unsuported geometry type";
            return;
    }
}

void SSceneComCollider::transformGeometryPlane() {
    SSceneComTransform* transform = _Owner->getTransform();
    if (!transform) {
        /* Transform component dose not exists */
        LOG(ERROR) << "Collider component exists on scene object without transform component";
        return;
    }
    Plane* plane = dynamic_cast<Plane*> (_Geometry);

    plane->setSize(_PlaneSize.mult(transform->getScaleAbs()));
    plane->setPosition(transform->getPositionAbs());
    plane->setAngle(transform->getAngle());
    if (transform->getParentAsPivot()) {
        plane->setPivot(transform->getPosition() * -1);
        plane->setPivotAngle(transform->getAngleAbs());
    }
    plane->transform();
}

bool SSceneComCollider::containsPoint(const Vector2& point) {
    if (!_Geometry) {
        return false;
    }
    bool isCont = _Geometry->containsPoint(point);
    return isCont;
}

Geometry* SSceneComCollider::getGeometry() {
    return _Geometry;
}