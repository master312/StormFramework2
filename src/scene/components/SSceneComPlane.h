#pragma once
#include "../SSceneComponent.h"
#include "../../core/utils/StormVertex.h"
#include "../../core/utils/Plane.h"

class SSceneComTransform;

class SSceneComPlane : public SSceneComponent {
public:
    SSceneComPlane(StormSceneObject* owner);
    virtual ~SSceneComPlane();

    /* Saves component data to @node */
    virtual void serializeXml(pugi::xml_node& node);

    /* Loads component data from @node. 
     * Returns < 0 on error */
    virtual int deserializeXml(pugi::xml_node& node);

    /* Called after all components have been loaded */
    virtual int initialize(SSceneComponentSystem* system);

    /* Sets plane size in pixels. */
    void setSize(const Vector2 size);
    
    /* Returns plane size in pixels. */
    Vector2 getSize() const;

    /* Returns plane size with scale tranform calculated in. */
    Vector2 getSizeTransformed() const;

    /* Sets whether plane should inherit rotation from parent transfrorm or not. */
    void setInheritRotation(bool value);

    /* Returns whether plane is inheriting rotation from parent. */
    bool getInheritRotation() const;

    /* Sets whether plane should inherit Scale from parent transfrorm or not. */
    void setInheritScale(bool value);
    
    /* Returns whether plane is inheriting Scale from parent. */
    bool getInheritScale() const;
    
    /* Returns pointer to vertices array.
     * WARNING: Vertices are updated in @transform() method. */
    StormVertex* getVertices();
    
    bool isRenderDebug();
    void setRenderDebug(bool shouldRender);
    
    /* Observer method. Called when transform component fires event.
     * Used to generate vertices and calculate transformed size. */
    void observeTransformChanged(void* data);

    /* Check if plane cointains point @point */
    bool containsPoint(const Vector2& point);

private:
    /* Plane size. */
    Vector2 _Size;

    /* Size multiplied by transforom scale */
    Vector2 _SizeTransformed;

    /* Math plane class that handles all calculations */
    Plane _Plane;

    /* Is rotation inherited from parent */
    bool _InheritRotation;

    /* Is scale inherited from parrent */
    bool _InheritScale;
    
    /* Should debug bounds for this plane be rendered. */
    bool _RenderDebug;

    /* Calculate vertices based on center coordinates */
    void calculateVertices(const Vector2& center);

    void transformRotation(const Vector2& centerPosition);
    void transformRotationWithParent();
};
