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

    /* Do transfrom calculations. */
    void transform();

    /* Returns position relative to parent
     * If parent is not set, this will return absolute position */
    Vector2 getPosition();

    /* Sets position transform
     * If parent exists, this position will be relative to parents position */
    void setPosition(Vector2 position);

    /* Returns absolute position on scene */
    Vector2 getPositionAbs();

    /* Returns transform scale multiplier */
    Vector2 getScale();
    
    /* Sets scale multiplier */
    void setScale(Vector2 scale);

    /* Returns rotation angle */
    float getAngle();

    /* Get absolute angle, with parent transformations included */
    float getAngleAbs();

    /* Sets rotation angle */
    void setAngle(float angle);
    
    /* Returns true if any of transform parameters ware changed */
    bool isChanged();

    /* Observer method. Called when owner's parent changes */
    void observeParentChanged(void* data);

    /* Observer method. Called when parent's transform has been changed */
    void observeParentTransformChanged(void* data);

private:
    /* Pointer to parent object's transform component */
    SSceneComTransform* _ParentTransform;

    /* Position relative to parent
     * If parent is not set, this is absolute position */
    Vector2 _Position;
    
    /* Absolute position on scene */
    Vector2 _PositionAbs;
    
    /* Scale multiplier */
    Vector2 _Scale;

    /* Rotation angle */
    float _Angle;

    /* Set to true if any of transform parameters ware changed.
     * Variable is set back to false in @transform() method. */
    bool _IsChanged;

    /* Pull transformation component from object's parent, 
     * and sets @_ParentTransform to its pointer. 
     * Or clears pointer to nullptr if parent was removed */
    void pullParentTransform();
};