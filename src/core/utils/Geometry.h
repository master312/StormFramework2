#pragma once
#include "math/Vector2.h"

enum GeometryType {
    GEOMETRY_TYPE_NONE,
    GEOMETRY_TYPE_PLANE,
    GEOMETRY_TYPE_CIRCLE
};

class Geometry {
public:
    Geometry();
    virtual ~Geometry();

    GeometryType getType();

    /* Sets and absolute position to geometry. */
    virtual void setPosition(const Vector2& position);
    virtual void setPositionX(float x);
    virtual void setPositionY(float y);

    virtual Vector2 getPosition() const;
    virtual float getPositionX();
    virtual float getPositionY();

    virtual void transform() = 0;

    /* Returns true if geometry contains point @point */
    virtual bool containsPoint(const Vector2& point) = 0;

protected:
    GeometryType _Type;

    Vector2 _Position;

    /* Flag set to true, when any property changes.
     * Reseted to false in Geometry::transform() */
    bool _HasChanged;
};
