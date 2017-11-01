#pragma once
#include <vector>
#include <map>
#include "../SSceneComponentSystem.h"
#include "../../core/resources/StormResourceFile.h"
#include "../../core/utils/math/Rect.h"
#include "../../libs/pugixml/pugixml.hpp"

/* System used for rendering sprites.
 * TODO: Optimize: Create sperated std::vector for animated sprites
 *                 and only iterate trough them in tick() method.
 *               : Identify SComSpriteSheet by some ID instead of filename
 *                 for better performance.
 *               : In ::loadSpriteSheetFromXml method, sort frames by ID
 *                 when loading, so that thay are independent from order in XML file.
 * */

class SSceneComSprite;

struct SComSpriteSheet {
    std::string filename;
    std::vector<Rect> frames;
    SComSpriteSheet() : filename("") { }
    SComSpriteSheet(const std::string& _filename) : filename(_filename) { }
    
    /* Retunrs count of frames in this sprite sheet */
    inline uint32_t count() { static_cast<uint32_t>(frames.size()); }
};

class SSceneSystemSprite : public SSceneComponentSystem {
public:
    SSceneSystemSprite();
    virtual ~SSceneSystemSprite();

    virtual void addComponent(SSceneComponent* component);

    virtual void tick(float deltaTime);
    
    virtual void render(StormRenderer* renderer);

    /* Returns pointer to sprite sheet object, or nullptr if not found */
    SComSpriteSheet* getSpriteSheet(const std::string& filename);

private:
    /* Vector containing all components managed by this system */
    std::vector<SSceneComSprite*> _SpriteComponents;

    /* All sprite sheets, indexed by filename */
    std::map<std::string, SComSpriteSheet> _SpriteSheets;

    /* Load sprite sheet from xml file @file, and add
     * it to @_SpriteSheets map.
     * Return < 0 on error */
    int loadSpriteSheetFromXml(spStormResourceFile file);

    /* Ticks animation for @sprite */
    void tickSpriteAnimation(SSceneComSprite* sprite, float deltaTime);

    /* Renders SSceneComSprite object to renderer */
    void renderSprite(SSceneComSprite* sprite, StormRenderer* renderer);
};