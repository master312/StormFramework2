#include "SComSprite.h"
#include "SSystemSprite.h"
#include "StormEngine.h"
#include "STextureManager.h"
#include "scene/components/transform/SComTransform.h"
#include "scene/SSceneObject.h"
#include "utils/math/ScalarMath.h"

SComSprite::SComSprite(SSceneObject* owner) : SSceneComponent(owner) {
    _Texture = nullptr;
    _ColorMultiply.set(255, 255, 255, 255);
    _ColorAdd.set(0, 0, 0, 0);
    _CurrentFrame = 0.0f;
    _FrameTime = 0.0f;
    _SpriteSheet = nullptr;
    _SpriteSheetFilename = "";
    _OriginalTextureName = "";
    _LastFrameTime = 0;
    _AnimationSpeed = 1.0f;

    _ScriptHandlerName = "sprite";
    _Type = S_SCENE_OBJECT_COM_SPRITE;
}

SComSprite::~SComSprite() {
    _Texture = nullptr;
}

void SComSprite::serializeXml(pugi::xml_node& node) {
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

    node.append_attribute("width").set_value(_RenderPlane.getSizeX());
    node.append_attribute("height").set_value(_RenderPlane.getSizeY());
    
    if (_AnimationSpeed != 1.0f) {
        node.append_attribute("animation_speed").set_value(_AnimationSpeed);
    }

    if (_SpriteSheetFilename != "") {
        node.append_attribute("sprite_sheet").set_value(_SpriteSheetFilename.c_str());
    }
}

int SComSprite::deserializeXml(pugi::xml_node& node) {
    SSceneComponent::deserializeXml(node);

    _ColorMultiply.r = (uint8_t)node.attribute("color_mult_r").as_int(255);
    _ColorMultiply.g = (uint8_t)node.attribute("color_mult_g").as_int(255);
    _ColorMultiply.b = (uint8_t)node.attribute("color_mult_b").as_int(255);
    _ColorMultiply.a = (uint8_t)node.attribute("color_mult_a").as_int(255);

    _ColorAdd.r = (uint8_t)node.attribute("color_add_r").as_int(0);
    _ColorAdd.g = (uint8_t)node.attribute("color_add_g").as_int(0);
    _ColorAdd.b = (uint8_t)node.attribute("color_add_b").as_int(0);
    _ColorAdd.a = (uint8_t)node.attribute("color_add_a").as_int(0);

    /* Custom plane size can be specified in XML */
    _RenderPlane.setSizeX(node.attribute("width").as_float(-1.0f));
    _RenderPlane.setSizeY(node.attribute("height").as_float(-1.0f));

    if (_RenderPlane.getSizeX() < 0.0f) {
        _RenderPlane.setSizeX(0.5f);
        LOG(WARNING) << "No width specified for sprite. Using default. OBJ ID: " << getOwner()->getId();
    }
    if (_RenderPlane.getSizeY() < 0.0f) {
        _RenderPlane.setSizeY(0.5f);
        LOG(WARNING) << "No height specified for sprite. Using default. OBJ ID: " << getOwner()->getId();
    }
    
    _AnimationSpeed = node.attribute("animation_speed").as_float(1.0f);

    _SpriteSheetFilename = node.attribute("sprite_sheet").as_string("");
    _OriginalTextureName = node.attribute("texture").as_string("");

    if (_OriginalTextureName == "" && _SpriteSheetFilename == "") {
        LOG(WARNING) << "Texture and sprite name not specified in XML file for SComSprite. Object ID " << getOwner()->getId();
    } else if (_OriginalTextureName != "") {
        _Texture = StormEngine::getTexture(_OriginalTextureName);
    }

    return 1;
}

int SComSprite::initialize(SSceneComponentSystem* system) {
    
    SComSprite::observeTransformChanged(nullptr);
    _Owner->registerObserver<SComSprite>(
            S_EVENT_TRANSFORM_UPDATED, &SComSprite::observeTransformChanged, this);
    
    if (_SpriteSheetFilename == "") {
        /* Set to nullptr again, just in case */
        _SpriteSheet = nullptr;
        return 0;
    }
    
    SSystemSprite* sysSprite = dynamic_cast<SSystemSprite*> (system);
    if (!sysSprite) {
        return -1;
    }

    _SpriteSheet = sysSprite->getSpriteSheet(_SpriteSheetFilename);
    if (!_SpriteSheet) {
        return -2;
    }
    _LastFrameTime = StormEngine::getTimeMs();
    if (!_Texture) {
        _Texture = StormEngine::getTexture(_SpriteSheet->textureName);
    }

    _FrameTime = 1000.0f / (float)_SpriteSheet->fps;
    return 1;
}

void SComSprite::observeTransformChanged(void* data) {
    SComTransform* transform = _Owner->getTransform();
    if (!transform) {
        LOG(ERROR) << "Sprite component exists on scene object without transform";
        return;
    }

    /* Store oritinal size for backup */
    const Vector2 originalSize = _RenderPlane.getSize();
    /* Scale size by transform scale */
    _RenderPlane.setSize(originalSize.mult(transform->getScaleAbs()));
    
    _RenderPlane.setPosition(transform->getPositionAbs());
    _RenderPlane.setAngle(transform->getAngle());
    
    if (transform->getParentAsPivot()) {
        _RenderPlane.setPivot(transform->getPosition() * -1);
        _RenderPlane.setPivotAngle(transform->getAngleAbs());
    } else {
        /* TODO: Micro - Optimization, dont clear pivot every time. */
        _RenderPlane.clearPivot();
    }
    
    _RenderPlane.transform();

    /* Restore original size after transformation */
    _RenderPlane.setSize(originalSize);
}

void SComSprite::setTexture(spStormTexture texture) {
    _Texture = texture;
}

spStormTexture SComSprite::getTexture() {
    return _Texture;
}

void SComSprite::setColorMultiply(const Color& color) {
    _ColorMultiply = color;
}

Color SComSprite::getColorMultiply() {
    return _ColorMultiply;
}

void SComSprite::setColorAdd(const Color& color) {
    _ColorAdd = color;
}

Color SComSprite::getColorAdd() {
    return _ColorAdd;
}

bool SComSprite::hasSpriteSheet() {
    return _SpriteSheet != nullptr;
}

float SComSprite::getCurrentFrame() const {
    return _CurrentFrame;
}

void SComSprite::setCurrentFrame(float count) {
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

float SComSprite::getFrameTime() const {
    return _FrameTime / _AnimationSpeed;
}

uint32_t SComSprite::getLastFrameTime() const {
    return _LastFrameTime;
}

void SComSprite::setLastFrameTime(uint32_t time) {
    _LastFrameTime = time;
}

std::reference_wrapper<const Rect> SComSprite::getCurrentFrameRect() const {
    if (!_SpriteSheet || !_SpriteSheet->frames.size()) {
        return _Texture->getArea();
    }
    return _SpriteSheet->frames[(int)getCurrentFrame()];
}

std::reference_wrapper<Plane> SComSprite::getRenderPlane() {
    return _RenderPlane;
}

const std::string& SComSprite::getSpriteSheetFilename() {
    return _SpriteSheetFilename;
}

SComSpriteSheet* SComSprite::getSpriteSheet() {
    return _SpriteSheet;
}

int SComSprite::getZ() const {
    return (_Owner->getTransform()->getAbsZ());
}
