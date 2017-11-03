#pragma once
#include "math/Vector2.h"
#include "StormVertex.h"

/* Class responsible for all plane related math calculations */

class Plane {
public:
    /* TODO: Constructors */
    Plane();
    ~Plane();

    /* Sets and absolute position to be used as center of plane. */
    void setPosition(const Vector2& position);
    void setPositionX(float x);
    void setPositionY(float y);

    Vector2 getPosition() const;
    float getPositionX();
    float getPositionY();

    /* Sets pivot position. This position is relative to plane position. */
    void setPivot(const Vector2& position);
    void setPivotX(float x);
    void setPivotY(float y);

    Vector2 getPivot() const;
    float getPivotX();
    float getPivotY();

    void setSize(const Vector2& position);
    void setSizeX(float x);
    void setSizeY(float y);

    Vector2 getSize() const;
    float getSizeX();
    float getSizeY();
    
    void setAngle(float angle);
    float getAngle();

    void setPivotAngle(float angle);
    float getPivotAngle();

    /* Do transform calculations */
    void transform();

    /* Returns true if plane contains point @point */
    bool containsPoint(const Vector2& point);

    /* Returns all 4 plane vertices.
     * Make sure to use Plane::transform() to update vertices position.
     * [0] -> Top Left, [1] -> Top Right, [2] -> Bottom right */
    StormVertex* getVertices();
    
private:
    Vector2 _Size;

    /* Position of plane center point */
    Vector2 _Position;
    
    /* Pivot positon, relative to center position */
    Vector2 _PivotPosition;
    
    float _Angle;
    
    float _PivotAngle;

    /* Flag set to true, when any property changes.
     * Reseted to false in Plane::transform() */
    bool _HasChanged;

    StormVertex _Vertices[4];
    
    void transformRotationAroundPivot();
    void transformRotationAroundCenter();

    void calculateVertices(const Vector2& center);
};