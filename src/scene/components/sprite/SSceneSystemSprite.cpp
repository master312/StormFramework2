#include "SSceneSystemSprite.h"
#include "SSceneComSprite.h"
#include "StormEngine.h"
#include "scene/SSceneObject.h"

SSceneSystemSprite::SSceneSystemSprite(SScene* scene) : SSceneComponentSystem(scene) {
    _Type = S_SCENE_OBJECT_COM_SPRITE;
}

SSceneSystemSprite::~SSceneSystemSprite() {
}

void SSceneSystemSprite::addComponent(SSceneComponent* component) {
    if (!validateComponent(component, S_SCENE_OBJECT_COM_SPRITE)) {
        return;
    }
    SSceneComponentSystem::addComponent(component);

    SSceneComSprite* com = dynamic_cast<SSceneComSprite*>(component);

    const std::string& spriteFilename = com->getSpriteSheetFilename();
    if (spriteFilename != "") {
        /* This component uses sprite sheet */
        if (loadSpriteSheetFromXml(StormEngine::getResource(spriteFilename)) < 0) {
            LOG(ERROR) << "Could not load sprite sheet '" << spriteFilename << "'";
        }
    }
    
    _SpriteComponents.push_back(com);
}

void SSceneSystemSprite::tick(float deltaTime) {
    for (size_t i = 0; i < _SpriteComponents.size(); i++) {
        SSceneComSprite* sprite = _SpriteComponents[i];
        if (sprite->hasSpriteSheet()) {
            tickSpriteAnimation(sprite, deltaTime);
        }
    }
}

void SSceneSystemSprite::render(StormRenderer* renderer) {
    for (size_t i = 0; i < _SpriteComponents.size(); i++) {
        renderSprite(_SpriteComponents[i], renderer);
    }
}

SComSpriteSheet* SSceneSystemSprite::getSpriteSheet(const std::string& filename) {
    auto iter = _SpriteSheets.find(filename);
    if (iter == _SpriteSheets.end()) {
        return nullptr; 
    }
    return &iter->second;
}

int SSceneSystemSprite::loadSpriteSheetFromXml(spStormResourceFile file) {
    if (!file) {
        LOG(ERROR) << "SSceneSystemSprite::loadSpriteSheetFromXml Resource file is nullptr";
        return -3;
    }

    SComSpriteSheet sprite(file->getFilenameWithExt());
    if (_SpriteSheets.find(sprite.filename) != _SpriteSheets.end()) {
        /* Sprite sheet already loaded */
        return 0;
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load(file->getBuffer(), file->getBufferSize());
    if (result.status != pugi::status_ok) {
        LOG(ERROR) << "Sprite XML " << file->getFilename() << " error: " << result.description();
        return -1;
    }

    pugi::xml_node rootNode = doc.child("sprite");
    if (rootNode.type() != pugi::node_element) {
        LOG(ERROR) << "Sprite XML " << file->getFilename() << " error. Missing root tag.";
        return -2;
    }

    sprite.fps = rootNode.attribute("fps").as_float(0.0f);
    sprite.textureName = rootNode.attribute("texture").as_string("");

#ifndef PRODUCTION
    if (sprite.fps == 0.0f) {
        LOG(WARNING) << "Sprite animation '" << sprite.filename << "' fps is 0.0";
    }
    if (sprite.textureName == "") {
        LOG(WARNING) << "Sprite animation '" << sprite.filename << "' no texture specified";
    }
#endif
    Point frameSize;
    frameSize.x = rootNode.attribute("frame_width").as_int(0);
    frameSize.y = rootNode.attribute("frame_height").as_int(0);
    if (frameSize.x > 0 && frameSize.y > 0) {
        /* There are no custom frames specified. 
         * Sprite texture is divided on equal portions */
        std::string direction = rootNode.attribute("direction").as_string("x");
        if (sprite.generateSpriteFramesFromSize(frameSize, direction == "x") < 0) {
            return -3;
        }
    } else {
        pugi::xml_node child = rootNode.first_child();
        if (sprite.loadSpriteFrames(child) < 0) {
            return -4;
        }
    }

    _SpriteSheets[sprite.filename] = sprite;

    return 1;
}

void SSceneSystemSprite::tickSpriteAnimation(SSceneComSprite* sprite, float deltaTime) {
    uint32_t timeDiff = StormEngine::getTimeMs() - sprite->getLastFrameTime();
    float framesDiff = (float)timeDiff / sprite->getFrameTime();
    if (framesDiff >= 1.0f) {
        /* Enough time has passed. Move to next frame */
        float nextFrame = sprite->getCurrentFrame() + framesDiff;
        sprite->setCurrentFrame(nextFrame);
        sprite->setLastFrameTime(StormEngine::getTimeMs());
    }
}

void SSceneSystemSprite::renderSprite(SSceneComSprite* sprite, StormRenderer* renderer) {
    /* TODO: Rewrite this. This is only prototype code */

    Plane& plane = sprite->getRenderPlane();
    Vector2* points = plane.getPoints();
    static StormVertex vertices[4];
    for (int i = 0; i < 4; i++) {
        vertices[i].position = points[i];
    }
    spStormTexture texture = sprite->getTexture();
    if (!texture) {
        return;
    }

    renderer->begin(S_RENDER_TRIANGLE_FAN);
    renderer->bindTexture(texture.get());
    renderer->setColorMultiply(sprite->getColorMultiply());
    renderer->setColorAdd(sprite->getColorAdd());

    if (sprite->hasSpriteSheet()) {
        /* Animated sprite sheet found. Calculate UV coordinates. */
        const Rect& rect = sprite->getCurrentFrameRect();
        const Point& textureSize = texture->getSize();

        vertices[0].uv.x = (float)rect.pos.x / (float)textureSize.x;
        vertices[0].uv.y = (float)rect.pos.y / (float)textureSize.y;

        vertices[1].uv.x = ((float)rect.pos.x / (float)textureSize.x) + ((float)rect.size.x / (float)textureSize.x);
        vertices[1].uv.y = vertices[0].uv.y;

        vertices[2].uv.x = vertices[1].uv.x;
        vertices[2].uv.y = ((float)rect.pos.y / (float)textureSize.y) + ((float)rect.size.y / (float)textureSize.y);

        vertices[3].uv.x = vertices[0].uv.x;
        vertices[3].uv.y = vertices[2].uv.y;
    } else {
        /* No sprite animation found. Render whole texture */
        vertices[0].uv.set(0.0f, 0.0f);
        vertices[1].uv.set(1.0f, 0.0f);
        vertices[2].uv.set(1.0f, 1.0f);
        vertices[3].uv.set(0.0f, 1.0f);
    }

    renderer->bindVertexData(vertices, 4);
    
    /* TODO: Dont bind indices like this */
    uint32_t indices[] = {0, 1, 2, 3};
    renderer->bindIndexData(indices, 4);
    
    renderer->draw();
}


int SComSpriteSheet::loadSpriteFrames(pugi::xml_node& node) {
    for (pugi::xml_node frameNode = node; frameNode; frameNode = frameNode.next_sibling()) {
        if (frameNode.type() != pugi::node_element) {
            continue;
        }

        int id = frameNode.attribute("id").as_int(-1);
        if (id < 0) {
            LOG(ERROR) << "Error 1 while parsing sprite sheet xml file.";
            return -1;
        }

        Rect rect;
        rect.pos.x = frameNode.attribute("startX").as_int(-1);
        rect.pos.y = frameNode.attribute("startY").as_int(-1);
        rect.size.x = frameNode.attribute("width").as_int(-1);
        rect.size.y = frameNode.attribute("height").as_int(-1);
        if (rect.size.x < 0 || rect.size.y < 0 || rect.pos.x < 0 || rect.pos.y < 0) {
            LOG(ERROR) << "Error 2 while parsing sprite sheet xml file.";
            return -2;
        }
        
        frames.push_back(rect);
    }
    return 1;
}

int SComSpriteSheet::generateSpriteFramesFromSize(const Point& frameSize, bool directionX) {
    spStormTexture texture = StormEngine::getTexture(textureName);
    if (!texture) {
        LOG(ERROR) << "Could not load sprite. Texture not found";
        return -1;
    }

    const Point& textureSize = texture->getSize();
    Point framesCount(textureSize.x / frameSize.x, textureSize.y / frameSize.y);

    if (!directionX) {
        std::swap(framesCount.x, framesCount.y);
    }
    
    for (int i = 0; i < framesCount.y; i++) {
        for (int j = 0; j < framesCount.x; j++) {
            Rect rect;
            if (directionX) {
                rect.pos.set(j * frameSize.x, i * frameSize.y);
            } else {
                rect.pos.set(i * frameSize.x, j * frameSize.y);
            }
            rect.size = frameSize;
            frames.push_back(rect);
        }
    }

    return 1;
}