#pragma once
#include "../SSceneComponent.h"
#include "../../core/utils/StormVertex.h"
#include "../../core/utils/Plane.h"

class SSceneComPlane : public SSceneComponent {
public:
    SSceneComPlane(StormSceneObject* owner);
    virtual ~SSceneComPlane();

    /* Saves component data to @node */
    virtual void serializeXml(pugi::xml_node& node);

    /* Loads component data from @node. 
     * Returns < 0 on error */
    virtual int deserializeXml(pugi::xml_node& node);

    /* Do transform calculations. */
    void transform(Plane* parent = nullptr);

    void setPosition(const Vector2& position);
    Vector2 getPosition() const;
    Vector2 getPositionTransformed() const;

    void setCenterPosition(const Vector2& position);
    Vector2 getCenterPosition() const;
    Vector2 getCenterPositionTransformed() const;

    void setAngle(float angle);
    void addAngle(float angle);
    float getAngle();

    /* Sets plane size in pixels */
    void setSize(const Vector2& size);
    /* Returns plane size */
    Vector2 getSize() const;
    /* Returns plane size with transform calculations calculated in */
    Vector2 getSizeTransformed() const;

    /* Sets plane scaling */
    void setScale(const Vector2& scale);
    Vector2 getScale() const;

    /* Returns pointer to vertices array.
     * WARNING: Vertices are updated in @transform() method. */
    StormVertex* getVertices();
    
private:
    /* Pivot position */
    Vector2 _PivotPosition;
    /* Plane center position relative to pivot position  */
    Vector2 _PlaneCenterPosition;

    /* Plane rotation angle around the pivot */
    float _Angle;

    /* Plane size */
    Vector2 _Size;
    /* Plane scale */
    Vector2 _Scale;

    /* Pivot position with transformations calculated in */
    Vector2 _PivotPositionTransformed;
    /* Offset from pivot to plane center with transfrom calculations calculated in */
    Vector2 _PlaneCenterTransformed;
    /* Plane size with transformations calculated in */
    Vector2 _SizeTransformed;

    /* Plane vertices, with transformations calculated in */
    StormVertex _Vertices[4];
    
    void transformScale(Plane* parent);
};
