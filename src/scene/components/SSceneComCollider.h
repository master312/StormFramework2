#pragma once
#include "../SSceneComponent.h"
#include "../../core/utils/Geometry.h"

class SSceneComCollider : public SSceneComponent {
public:
    SSceneComCollider(StormSceneObject* owner);
    virtual ~SSceneComCollider();

    virtual void serializeXml(pugi::xml_node& node);

    virtual int deserializeXml(pugi::xml_node& node);

    virtual int initialize(SSceneComponentSystem* system);

    /* Observer method. Called when transform component fires event.
     * Used to handle geometry transformations. */
    void observeTransformChanged(void* data);

    /* Returns true if collider contains @point */
    bool containsPoint(const Vector2& point);

    Geometry* getGeometry();

private:
    Geometry* _Geometry;

    /* Used if @_Geometry is plane, to store original size */
    Vector2 _PlaneSize;

    void transformGeometryPlane();
}; 
