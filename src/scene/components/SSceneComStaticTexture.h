#pragma once
#include "../SSceneComponent.h"
#include "../../core/graphics/StormTexture.h"
#include "../../core/graphics/StormRenderer.h"
/* Component for drawing static texture on screen */

class SSceneComPlane;

class SSceneComStaticTexture : public SSceneComponent {
public:
    SSceneComStaticTexture(StormSceneObject* owner);
    virtual ~SSceneComStaticTexture();

    /* Saves component data to @node */
    virtual void serializeXml(pugi::xml_node& node);

    /* Loads component data from @node. 
     * Returns < 0 on error */
    virtual int deserializeXml(pugi::xml_node& node);

    void setPlaneComponent(SSceneComPlane* plane);
    SSceneComPlane* getPlaneComponent();

    void setTexture(spStormTexture texture);
    spStormTexture getTexture();

    void setColorMultiply(Color color);
    Color getColorMultiply();
    /* Method used by editor. */
    Color* getColorMultiplyPtr();

    void setColorAdd(Color color);
    Color getColorAdd();
    /* Method used by editor. */
    Color* getColorAddPtr();

    void render(StormRenderer* renderer);

private:
    /* Reference to plane component */
    SSceneComPlane* _PlaneComponent;
    spStormTexture _Texture;

    Color _ColorMultiply;
    Color _ColorAdd;
};
