#pragma once
#include "../SSceneComponent.h"
#include "../../core/utils/math/Vector2.h"

/* This component holds scene object tranform info - Position, rotation and scale.
 * Parent transformation calculations are also handled here. */

class SSceneComTransform : public SSceneComponent {
public:
    SSceneComTransform(StormSceneObject* owner);
    virtual ~SSceneComTransform();

    /* Saves component data to @node */
    virtual void serializeXml(pugi::xml_node& node);

    /* Loads component data from @node. 
     * Returns < 0 on error */
    virtual int deserializeXml(pugi::xml_node& node);

    /* Initialize component. Called after all components have been loaded */
    virtual void initialize();

    /* Do transfrom calculations with @parent transformations calculated in */
    void transform(SSceneComTransform* parent);

    /* Returns position relative to parent
     * If parent is not set, this will return absolute position */
    Vector2 getPosition();
    Vector2* getPositionPtr();

    /* Sets position transform
     * If parent exists, this position will be relative to parents position */
    void setPosition(Vector2 position);

    /* Returns absolute position on scene */
    Vector2 getPositionAbs();
    Vector2* getPositionAbsPtr();

    /* Returns transform scale multiplier */
    Vector2 getScale();
    Vector2* getScalePtr();
    
    /* Sets scale multiplier */
    void setScale(Vector2 scale);

    /* Returns rotation angle */
    float getAngle();
    float* getAnglePtr();

    /* Sets rotation angle */
    void setAngle(float angle);
    
private:
    /* Position relative to parent
     * If parent is not set, this is absolute position */
    Vector2 _Position;
    
    /* Absolute position on scene */
    Vector2 _PositionAbs;
    
    /* Scale multiplier */
    Vector2 _Scale;

    /* Rotation angle */
    float _Angle;
};