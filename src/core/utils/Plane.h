#pragma once
#include "math/Vector2.h"

/* Actual plane is linked to pivot. Pivot is used as 'main position', and actual plane is relative to it. */

class Plane {
public:
    /* TODO: Proper constructors */
    Plane();
    ~Plane();

    /* Do transform calculations. */
    void transform(Plane* parent = nullptr);

    void setPosition(const Vector2& position);
    Vector2 getPosition() const;
    Vector2 getPositionTransformed() const;
    
    /* Sets offset from pivot to plane */
    void setPlaneOffset(const Vector2& offset);
    Vector2 getPlaneOffset() const;
    Vector2 getPlaneOffsetTransformed() const;

    void setAngle(float angle);
    void addAngle(float angle);
    float getAngle();

    /* Sets plane size in units (most likely pixels) */
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
    Vector2* getVertices();
    /* Returns pointer to specific vertex */
    Vector2* getVertex(int ver);
private:
    /* Pivot position */
    Vector2 _PivotPosition;
    /* Offset from pivot to plane center */
    Vector2 _PlanePivotOffset;

    /* Pivot position with transform calculations calculated in */
    Vector2 _PivotPositionTransformed;
    /* Offset from pivot to plane center with transfrom calculations calculated in */
    Vector2 _PlanePivotOffsetTransformed;
    
    /* Plane rotation angle around the pivot */
    float _Angle;

    /* Plane size in abs units (most likely pixels) */
    Vector2 _Size;
    /* Plane scale */
    Vector2 _Scale;
    /* Plane size in abs units (most likely pixels) with transformation calculations calculated in */
    Vector2 _SizeTransformed;

    /* Plane vertices, with transformations calculated in */
    Vector2 _Vertices[4];

    void transformScale(Plane* parent);
};