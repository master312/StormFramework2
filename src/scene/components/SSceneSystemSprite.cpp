#include "SSceneSystemSprite.h"
#include "SSceneComSprite.h"
#include "../StormSceneObject.h"
#include "../../StormEngine.h"

SSceneSystemSprite::SSceneSystemSprite() {
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
        return -4;
    }

    pugi::xml_node rootNode = doc.child("sprite");
    if (rootNode.type() != pugi::node_element) {
        LOG(ERROR) << "Sprite XML " << file->getFilename() << " error. Missing root tag.";
        return -5;
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

    for (pugi::xml_node frameNode = rootNode.first_child(); frameNode; frameNode = frameNode.next_sibling()) {
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
        
        sprite.frames.push_back(rect);
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
    Plane& plane = sprite->getRenderPlane();
    StormVertex* vertices = plane.getVertices();
    spStormTexture texture = sprite->getTexture();

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
    }

    renderer->bindVertexData(vertices, 4);
    
    /* TODO: Dont bind indices like this */
    uint32_t indices[] = {0, 1, 2, 3};
    renderer->bindIndexData(indices, 4);
    
    renderer->draw();
}
