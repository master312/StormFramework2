#include "SSceneComStaticTexture.h"
#include "SSceneComPlane.h"
#include "../../StormTextureManager.h"
#include "../StormSceneObject.h"
#include "../../StormEngine.h"

SSceneComStaticTexture::SSceneComStaticTexture(StormSceneObject* owner) : SSceneComponent(owner) {
    _PlaneComponent = nullptr;
    _Texture = nullptr;
    _ColorMultiply.set(255, 255, 255, 255);
    _ColorAdd.set(0, 0, 0, 0);
    
    _Type = S_SCENE_OBJECT_COM_STATIC_TEXTURE;
}

SSceneComStaticTexture::~SSceneComStaticTexture() {
    _PlaneComponent = nullptr;
    _Texture = nullptr;
}

void SSceneComStaticTexture::serializeXml(pugi::xml_node& node) {
    SSceneComponent::serializeXml(node);

    if (!_Texture->isDefaultTexture()) {
        node.append_attribute("texture").set_value(_Texture->getName().c_str());
    }

    node.append_attribute("color_mult_r").set_value((int)_ColorMultiply.r);
    node.append_attribute("color_mult_g").set_value((int)_ColorMultiply.g);
    node.append_attribute("color_mult_b").set_value((int)_ColorMultiply.b);
    node.append_attribute("color_mult_a").set_value((int)_ColorMultiply.a);

    node.append_attribute("color_add_r").set_value((int)_ColorAdd.r);
    node.append_attribute("color_add_g").set_value((int)_ColorAdd.g);
    node.append_attribute("color_add_b").set_value((int)_ColorAdd.b);
    node.append_attribute("color_add_a").set_value((int)_ColorAdd.a);
}

int SSceneComStaticTexture::deserializeXml(pugi::xml_node& node) {
    SSceneComponent::deserializeXml(node);

    std::string textureName = node.attribute("texture").as_string("");
    if (textureName == "") {
        LOG(WARNING) << "Texture name not specified in XML file for SSceneComStaticTexture. Object ID " << getOwner()->getId();
    }
    _Texture = StormEngine::getModule<StormTextureManager>()->getTexture(textureName);

    _ColorMultiply.r = (uint8_t)node.attribute("color_mult_r").as_int(255);
    _ColorMultiply.g = (uint8_t)node.attribute("color_mult_g").as_int(255);
    _ColorMultiply.b = (uint8_t)node.attribute("color_mult_b").as_int(255);
    _ColorMultiply.a = (uint8_t)node.attribute("color_mult_a").as_int(255);

    _ColorAdd.r = (uint8_t)node.attribute("color_add_r").as_int(0);
    _ColorAdd.g = (uint8_t)node.attribute("color_add_g").as_int(0);
    _ColorAdd.b = (uint8_t)node.attribute("color_add_b").as_int(0);
    _ColorAdd.a = (uint8_t)node.attribute("color_add_a").as_int(0);

    return 1;
}

void SSceneComStaticTexture::setPlaneComponent(SSceneComPlane* plane) {
    _PlaneComponent = plane;
}

SSceneComPlane* SSceneComStaticTexture::getPlaneComponent() {
    return _PlaneComponent;
}

void SSceneComStaticTexture::setTexture(spStormTexture texture) {
    _Texture = texture;
}

spStormTexture SSceneComStaticTexture::getTexture() {
    return _Texture;
}

void SSceneComStaticTexture::setColorMultiply(Color color) {
    _ColorMultiply = color;
}

Color SSceneComStaticTexture::getColorMultiply() {
    return _ColorMultiply;
}

Color* SSceneComStaticTexture::getColorMultiplyPtr() {
    return &_ColorMultiply;
}

void SSceneComStaticTexture::setColorAdd(Color color) {
    _ColorAdd = color;
}

Color SSceneComStaticTexture::getColorAdd() {
    return _ColorAdd;
}

Color* SSceneComStaticTexture::getColorAddPtr() {
    return &_ColorAdd;
}
