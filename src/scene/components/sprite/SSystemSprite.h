#pragma once
#include <vector>
#include <map>
#include "scene/SSceneComponentSystem.h"
#include "resources/StormResourceFile.h"
#include "utils/math/Rect.h"
#include "pugixml/pugixml.hpp"

/* System used for rendering sprites.
 * TODO: Optimize: Create sperated std::vector for animated sprites
 *                 and only iterate trough them in tick() method.
 *               : Identify SComSpriteSheet by some ID instead of filename
 *                 for better performance.
 *               : In ::loadSpriteSheetFromXml method, sort frames by ID
 *                 when loading, so that thay are independent from order in XML file.
 * */

class SComSprite;

struct SComSpriteSheet {
    std::string filename;
    std::vector<Rect> frames;
    float fps;
    std::string textureName;
    SComSpriteSheet() : filename(""), fps(0.0f), textureName("") { }
    SComSpriteSheet(const std::string& _filename, float _fps = 0.0f) : 
        filename(_filename), fps(_fps), textureName("") { }
    
    /* Load all frames (child nodes) from &node and add them to @sprite.
     * Returns < 0 on error */
    int loadSpriteFrames(pugi::xml_node& node);
    
    /* Generate sprite frames from frame size and texture size.
     * Texture filename must be set before using this method.
     * if @directionX parameter is set, then frames are going in order on X axes. 
     * If not, then do the same thing on y axes.
     * Returns < 0 on error */
    int generateSpriteFramesFromSize(const Point& textureSize, bool directionX);

    /* Retunrs count of frames in this sprite sheet */
    inline uint32_t count() { return static_cast<uint32_t>(frames.size()); }
};

class SSystemSprite : public SSceneComponentSystem {
public:
    SSystemSprite(SScene* scene);
    virtual ~SSystemSprite();

    virtual void tick(float deltaTime);
    
    virtual void render(StormRenderer* renderer);

    virtual void onComponentAdded(SSceneComponent* component);

    virtual void onComponentRemoved(SSceneComponent* component);

    /* Returns pointer to sprite sheet object, or nullptr if not found */
    SComSpriteSheet* getSpriteSheet(const std::string& filename);

private:
    /* Vector containing all components managed by this system */
    std::vector<SComSprite*> _SpriteComponents;

    /* All sprite sheets, indexed by filename */
    std::map<std::string, SComSpriteSheet> _SpriteSheets;


    /* Sort @_SpriteComponents by transform's Z coordinate */
    void sortComponentsByZ();

    /* Load sprite sheet from xml file @file, and add
     * it to @_SpriteSheets map.
     * Return < 0 on error */
    int loadSpriteSheetFromXml(spStormResourceFile file);

    /* Ticks animation for @sprite */
    void tickSpriteAnimation(SComSprite* sprite, float deltaTime);

    /* Renders SComSprite object to renderer */
    void renderSprite(SComSprite* sprite, StormRenderer* renderer);
};