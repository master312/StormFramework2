#include "SSceneComSprite.h"
#include "SSceneComPlane.h"
#include "SSceneSystemSprite.h"
#include "../StormSceneObject.h"
#include "../../StormTextureManager.h"
#include "../../StormEngine.h"
#include "../../core/utils/math/ScalarMath.h"

SSceneComSprite::SSceneComSprite(StormSceneObject* owner) : SSceneComponent(owner) {
    _Texture = nullptr;
    _ColorMultiply.set(255, 255, 255, 255);
    _ColorAdd.set(0, 0, 0, 0);
    _CurrentFrame = 0.0f;
    _FrameTime = 0.0f;
    _SpriteSheet = nullptr;
    _SpriteSheetFilename = "";
    _OriginalTextureName = "";
    _LastFrameTime = 0;

    _Type = S_SCENE_OBJECT_COM_SPRITE;
}

SSceneComSprite::~SSceneComSprite() {
    _Texture = nullptr;
}

void SSceneComSprite::serializeXml(pugi::xml_node& node) {
    SSceneComponent::serializeXml(node);
 
    if (_OriginalTextureName != "") {
        node.append_attribute("texture").set_value(_OriginalTextureName.c_str());
    }

    node.append_attribute("color_mult_r").set_value((int)_ColorMultiply.r);
    node.append_attribute("color_mult_g").set_value((int)_ColorMultiply.g);
    node.append_attribute("color_mult_b").set_value((int)_ColorMultiply.b);
    node.append_attribute("color_mult_a").set_value((int)_ColorMultiply.a);

    node.append_attribute("color_add_r").set_value((int)_ColorAdd.r);
    node.append_attribute("color_add_g").set_value((int)_ColorAdd.g);
    node.append_attribute("color_add_b").set_value((int)_ColorAdd.b);
    node.append_attribute("color_add_a").set_value((int)_ColorAdd.a);

    if (_SpriteSheetFilename != "") {
        node.append_attribute("sprite_sheet").set_value(_SpriteSheetFilename.c_str());
    }
}

int SSceneComSprite::deserializeXml(pugi::xml_node& node) {
    SSceneComponent::deserializeXml(node);

    _ColorMultiply.r = (uint8_t)node.attribute("color_mult_r").as_int(255);
    _ColorMultiply.g = (uint8_t)node.attribute("color_mult_g").as_int(255);
    _ColorMultiply.b = (uint8_t)node.attribute("color_mult_b").as_int(255);
    _ColorMultiply.a = (uint8_t)node.attribute("color_mult_a").as_int(255);

    _ColorAdd.r = (uint8_t)node.attribute("color_add_r").as_int(0);
    _ColorAdd.g = (uint8_t)node.attribute("color_add_g").as_int(0);
    _ColorAdd.b = (uint8_t)node.attribute("color_add_b").as_int(0);
    _ColorAdd.a = (uint8_t)node.attribute("color_add_a").as_int(0);

    _SpriteSheetFilename = node.attribute("sprite_sheet").as_string("");
    _OriginalTextureName = node.attribute("texture").as_string("");
    
    if (_OriginalTextureName == "" && _SpriteSheetFilename == "") {
        LOG(WARNING) << "Texture and sprite name not specified in XML file for SSceneComSprite. Object ID " << getOwner()->getId();
    } else if (_OriginalTextureName != "") {
        _Texture = StormEngine::getModule<StormTextureManager>()->getTexture(_OriginalTextureName);
    }

    return 1;
}

int SSceneComSprite::initialize(SSceneComponentSystem* system) {
    if (_SpriteSheetFilename == "") {
        /* Set to nullptr again, just in case */
        _SpriteSheet = nullptr;
        return 0;
    }
    
    SSceneSystemSprite* sysSprite = dynamic_cast<SSceneSystemSprite*> (system);
    if (!sysSprite) {
        return -1;
    }

    _SpriteSheet = sysSprite->getSpriteSheet(_SpriteSheetFilename);
    _LastFrameTime = StormEngine::getTimeMs();
    if (!_Texture) {
        _Texture = StormEngine::getModule<StormTextureManager>()->getTexture(_SpriteSheet->textureName);
    }
    /* TODO * speed scaler */
    _FrameTime = 1000 /_SpriteSheet->fps;
    return 1;
}

void SSceneComSprite::setTexture(spStormTexture texture) {
    _Texture = texture;
}

spStormTexture SSceneComSprite::getTexture() {
    return _Texture;
}

void SSceneComSprite::setColorMultiply(Color color) {
    _ColorMultiply = color;
}

Color SSceneComSprite::getColorMultiply() {
    return _ColorMultiply;
}

Color* SSceneComSprite::getColorMultiplyPtr() {
    return &_ColorMultiply;
}

void SSceneComSprite::setColorAdd(Color color) {
    _ColorAdd = color;
}

Color SSceneComSprite::getColorAdd() {
    return _ColorAdd;
}

Color* SSceneComSprite::getColorAddPtr() {
    return &_ColorAdd;
}

bool SSceneComSprite::hasSpriteSheet() {
    return _SpriteSheet != nullptr;
}

float SSceneComSprite::getCurrentFrame() const {
    return _CurrentFrame;
}

void SSceneComSprite::setCurrentFrame(float count) {
#ifndef PRODUCTION
    if (!_SpriteSheet) {
        LOG(ERROR) << "Could not set next frame. Sprite sheet is nullptr";
        return;
    }
#endif
    if (count >= (float)_SpriteSheet->count()) {
        _CurrentFrame = 0.0f;
    } else {
        _CurrentFrame = count;
    }
}

uint32_t SSceneComSprite::getFrameTime() const {
    return _FrameTime;
}

uint32_t SSceneComSprite::getLastFrameTime() const {
    return _LastFrameTime;
}

void SSceneComSprite::setLastFrameTime(uint32_t time) { 
    _LastFrameTime = time;
}

std::reference_wrapper<const Rect> SSceneComSprite::getCurrentFrameRect() const {
    if (!_SpriteSheet) {
        return _Texture->getArea();
    }

    return _SpriteSheet->frames[(int)getCurrentFrame()];
}

const std::string& SSceneComSprite::getSpriteSheetFilename() {
    return _SpriteSheetFilename;
}

SComSpriteSheet* SSceneComSprite::getSpriteSheet() {
    return _SpriteSheet;
}