#pragma once
#include "Geometry.h"
#include "StormVertex.h"

/* Class responsible for all plane related math calculations */

class Plane : public Geometry {
public:
    /* TODO: Constructors */
    Plane();
    virtual ~Plane();

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
    virtual void transform();

    /* Returns true if plane contains point @point */
    virtual bool containsPoint(const Vector2& point);

    /* Retunrs 4 points representing palne.
     * Make sure to use Plane::transform() to update vertices position.
     * [0] -> Top Left, [1] -> Top Right, [2] -> Bottom right */
    Vector2* getPoints();
    
private:
    Vector2 _Size;
    
    /* Pivot positon, relative to center position */
    Vector2 _PivotPosition;
    
    float _Angle;
    
    float _PivotAngle;

    /* 4 points representing plane */
    Vector2 _Points[4];
    
    void transformRotationAroundPivot();
    void transformRotationAroundCenter();

    void calculatePoints(const Vector2& center, bool flipX = false);
};