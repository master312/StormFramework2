#pragma once
#include "../../SSceneComponent.h"
#include "utils/math/Vector2.h"

/* This component holds scene object tranform info - Position, rotation and scale.
 * Parent transformation calculations are also handled here. */

class SSceneComTransform : public SSceneComponent {
public:
    SSceneComTransform(SSceneObject* owner);
    virtual ~SSceneComTransform();

    /* Saves component data to @node */
    virtual void serializeXml(pugi::xml_node& node);

    /* Loads component data from @node. 
     * Returns < 0 on error */
    virtual int deserializeXml(pugi::xml_node& node);

    /* Initialize component. Called after all components have been loaded */
    virtual int initialize(SSceneComponentSystem* system);

    /* Do transform calculations. */
    void transform();

    /* Returns position relative to parent
     * If parent is not set, this will return absolute position */
    Vector2 getPosition() const;

    /* Sets position transform
     * If parent exists, this position will be relative to parents position */
    void setPosition(const Vector2 position);

    /* Sets position transform
     * If parent exists, this position will be relative to parents position */
    void setPosition(float x, float y);

    /* Sets absolute position of this transform.
     * If transform has parent, @position will be converted to local coordinates.
     * TODO: Not fully tested */
    void setPositionAbs(const Vector2 position);

    /* Sets absolute position of this transform.
     * If transform has parent, @position will be converted to local coordinates.
     * TODO: Not fully tested */
    void setPositionAbs(float x, float y);

    /* Sets X local position */
    void setX(float x);

    /* Sets Y local position */
    void setY(float y);

    /* Gets local X position */
    float getX() const;

    /* Gets local Y position */
    float getY() const;

    /* Returns absolute position on scene */
    Vector2 getPositionAbs() const;

    /* Gets absolute X position */
    float getAbsX() const;

    /* Gets absolute Y position */
    float getAbsY() const;

    /* Returns transform scale multiplier */
    Vector2 getScale() const;
    
    /* Returns scale with parent scale calculations calculated in.
     * This will return same resoult as ::getScale() method
     * if Inherit Scale flag is not set */
    Vector2 getScaleAbs() const;

    /* Sets scale multiplier */
    void setScale(Vector2 scale);

    /* Returns Z depth of this component.
     * Z depth is mostly used to deside render priority for sprites and other
     * renderable components. */
    int getZ() const;

    /* Returns Z depth of this component, with parent transform calculated in.
     * Z depth is mostly used to deside render priority for sprites and other
     * renderable components. */
    int getAbsZ() const;

    /* Sets Z depth */
    void setZ(int value);

    /* Returns rotation angle */
    float getAngle() const;

    /* Get absolute angle, with parent transformations included */
    float getAngleAbs() const;

    /* Sets rotation angle */
    void setAngle(float angle);
    
    /* Sets whether scale should be inherited from parent */
    void setInheritScale(bool value);

    /* Returns whether scale is inherited from parent */
    bool getInheritScale() const;

    /* Sets whether parent should be used as pivot, while
     * performing transform calculations */
    void setParentAsPivot(bool value);

    /* Returns whether component is transformed using
     * parent as pivot. 
     * Will always return false if there is no parent set to owner object. */
    bool getParentAsPivot() const;

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

    /* Scale multiplier with parent scale calculated in */
    Vector2 _ScaleAbs;

    /* Z coordinate. Mostly used as render priority for sprites */
    int _ZPriority;

    /* Rotation angle */
    float _Angle;

    /* Set to true if scale should be inherited from parent */
    bool _InheritScale;

    /* If set to true, parent will act as pivot */
    bool _ParentAsPivot;

    /* Set to true if any of transform parameters ware changed.
     * Variable is set back to false in @transform() method. */
    bool _IsChanged;

    /* Pull transformation component from object's parent, 
     * and sets @_ParentTransform to its pointer. 
     * Or clears pointer to nullptr if parent was removed */
    void pullParentTransform();
};