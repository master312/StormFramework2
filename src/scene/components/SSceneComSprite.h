#pragma once
#include "../SSceneComponent.h"
#include "../../core/graphics/StormTexture.h"
#include "../../core/graphics/StormRenderer.h"
#include "../../core/utils/math/Rect.h"
#include "../../core/utils/Plane.h"

/* Component for drawing sprites on the screen */

/* Declared in SSceneSystemSprite.h */
struct SComSpriteSheet;

class SSceneComSprite : public SSceneComponent {
public:
    SSceneComSprite(StormSceneObject* owner);
    virtual ~SSceneComSprite();

    /* Saves component data to @node */
    virtual void serializeXml(pugi::xml_node& node);

    /* Loads component data from @node. 
     * Returns < 0 on error */
    virtual int deserializeXml(pugi::xml_node& node);

    virtual int initialize(SSceneComponentSystem* system);

    /* Observer method. Called when transform component fires event.
     * Used to handle plane transformations. */
    void observeTransformChanged(void* data);

    void setTexture(spStormTexture texture);
    spStormTexture getTexture();

    void setColorMultiply(const Color& color);
    Color getColorMultiply();

    void setColorAdd(const Color& color);
    Color getColorAdd();

    /* Returns true if there is sprite sheet attatched to this sprite component */
    bool hasSpriteSheet();

    /* Get current sprite frame. 
     * Always returns 0 if texture is not animated */
    float getCurrentFrame() const;
    
    /* Increases animation to next frame. 
     * Multiple frames can be ticked with by setting argument @count.
     * Clamps to begining if needed */
    void setCurrentFrame(float count);
    
    /* Returns how long one sprite frame lasts in milisecounds (ms). 
     * (animation speed - 1000 / fps / speed) */
    float getFrameTime() const;
    
    /* Returns time when was the last frame changed */
    uint32_t getLastFrameTime() const;

    /* Sets value to @_LastFrameTime. 
     * If @time == 0 (default argumet) current time will be set */
    void setLastFrameTime(uint32_t time);

    /* Returns reference texture part (rect) that should be rendered.
     * If sprite is animated, this rect will be current animation frame,
     * if not, rect will be set to size of whole texture. */
    std::reference_wrapper<const Rect> getCurrentFrameRect() const;

    /* Returns reference to plane with abs coordinates on which 
     * animation frame should be rendered. */
    std::reference_wrapper<Plane> getRenderPlane();

    const std::string& getSpriteSheetFilename();

    /* Returns pointer to sprite sheet object, or nullptr if not found */
    SComSpriteSheet* getSpriteSheet();

private:
    spStormTexture _Texture;

    /* Texture filename loaded from XML */
    std::string _OriginalTextureName;

    Color _ColorMultiply;
    Color _ColorAdd;

    /* Plane on which sprite will be rendered */ 
    Plane _RenderPlane;

    /* File name of sprite sheet xml. */
    std::string _SpriteSheetFilename;

    /* Reference to sprite sheet for faster access */
    SComSpriteSheet* _SpriteSheet;

    /* Animation speed (fps) multiplier */
    float _AnimationSpeed;

    /* Duration of one frame in milisecounds */
    float _FrameTime;

    /* Index of frame that is currently been drawn */
    uint32_t _CurrentFrame;
    
    /* Time in milisecounds when the last frame was changed */
    uint32_t _LastFrameTime;
};
