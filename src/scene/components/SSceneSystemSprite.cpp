#include "SSceneSystemSprite.h"
#include "SSceneComSprite.h"
#include "SSceneComPlane.h"
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
    uint64_t timeDiff = StormEngine::getTimeNs() - sprite->getLastFrameTime();
    if ((float)(timeDiff / 1000000000) >= sprite->getFrameTime()) {
        /* Enough time has passed. Move to next frame */
        uint32_t nextFrame = sprite->getCurrentFrame() + 1;
        sprite->setCurrentFrame(nextFrame);
        sprite->setLastFrameTime(StormEngine::getTimeNs());
    }
}

void SSceneSystemSprite::renderSprite(SSceneComSprite* sprite, StormRenderer* renderer) {
    /* 
     * Temporary rendering core. 
     * TODO: Rewrite this. This is only used for temporary debugging
     * */
    renderer->begin(S_RENDER_TRIANGLE_FAN);
    renderer->bindTexture(sprite->getTexture().get());
    renderer->setColorMultiply(sprite->getColorMultiply());
    renderer->setColorAdd(sprite->getColorAdd());
    StormVertex* vertices = sprite->getOwner()->getPlane()->getVertices();

    if (sprite->hasSpriteSheet()) {
        spStormTexture txt = sprite->getTexture();
        const Rect& rect = sprite->getCurrentFrameRect();

        vertices[0].uv.x = (float)rect.pos.x / (float)txt->getSize().x;
        vertices[0].uv.y = (float)rect.pos.y / (float)txt->getSize().y;

        vertices[1].uv.x = ((float)rect.pos.x / (float)txt->getSize().x) + ((float)rect.size.x / (float)txt->getSize().x);
        vertices[1].uv.y = (float)rect.pos.y / (float)txt->getSize().y;

        vertices[2].uv.x = ((float)rect.pos.x / (float)txt->getSize().x) + ((float)rect.size.x / (float)txt->getSize().x);
        vertices[2].uv.y = ((float)rect.pos.y / (float)txt->getSize().y) + ((float)rect.size.y / (float)txt->getSize().y);

        vertices[3].uv.x = (float)rect.pos.x / (float)txt->getSize().x;
        vertices[3].uv.y = ((float)rect.pos.y / (float)txt->getSize().y) + ((float)rect.size.y / (float)txt->getSize().y);
    }

    renderer->bindVertexData(sprite->getOwner()->getPlane()->getVertices(), 4);
    
    uint32_t indices[] = {0, 1, 2, 3};
    renderer->bindIndexData(indices, 4);
    
    renderer->draw();
}
