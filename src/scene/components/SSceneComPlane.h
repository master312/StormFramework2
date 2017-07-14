#pragma once
#include "../SSceneComponent.h"
#include "../../core/utils/StormVertex.h"

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
    virtual void initialize();

    /* Sets plane size in pixels. */
    void setSize(const Vector2 size);
    
    /* Returns plane size in pixels. */
    Vector2 getSize() const;

    /* Returns plane size with scale tranform calculated in. */
    Vector2 getSizeTransformed() const;

    /* Returns pointer to vertices array.
     * WARNING: Vertices are updated in @transform() method. */
    StormVertex* getVertices();
    
    bool isRenderDebug();
    void setRenderDebug(bool shouldRender);
    
    /* Event listener. Called by owner object when transform changes.
     * Used to generate vertices and calculate transformed size. */
    void onTransformChanged(void* data);

private:
    /* Pointer to owner's transform component for faster access */
    SSceneComTransform* _Transform;

    /* Plane size. */
    Vector2 _Size;
    Vector2 _SizeTransformed;

    /* Plane vertices, with transformations calculated in. */
    StormVertex _Vertices[4];
    
    /* Should debug bounds for this plane be rendered. */
    bool _RenderDebug;
};
