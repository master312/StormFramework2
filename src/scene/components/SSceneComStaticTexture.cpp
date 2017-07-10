#include "SSceneComStaticTexture.h"
#include "SSceneComPlane.h"
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
    node.append_attribute("color_r").set_value((int)_ColorMultiply.r);
    node.append_attribute("color_g").set_value((int)_ColorMultiply.g);
    node.append_attribute("color_b").set_value((int)_ColorMultiply.b);
    node.append_attribute("color_a").set_value((int)_ColorMultiply.a);
}

int SSceneComStaticTexture::deserializeXml(pugi::xml_node& node) {
    SSceneComponent::deserializeXml(node);

    std::string textureName = node.attribute("texture").as_string("");
    if (textureName == "") {
        LOG(WARNING) << "Texture name not specified in XML file for SSceneComStaticTexture. Object ID " << getOwner()->getId();
    }
    _Texture = StormEngine::instance()->getTextureManager()->getTexture(textureName);

    _ColorMultiply.r = (uint8_t)node.attribute("color_r").as_int(255);
    _ColorMultiply.g = (uint8_t)node.attribute("color_g").as_int(255);
    _ColorMultiply.b = (uint8_t)node.attribute("color_b").as_int(255);
    _ColorMultiply.a = (uint8_t)node.attribute("color_a").as_int(255);

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

void SSceneComStaticTexture::render(StormRenderer* renderer) {
    renderer->begin(S_RENDER_TRIANGLE_FAN);
    renderer->bindTexture(_Texture.get());
    renderer->setColorMultiply(_ColorMultiply);
    renderer->setColorAdd(_ColorAdd);
    renderer->bindVertexData(_PlaneComponent->getVertices(), 4);
    
    uint32_t indices[] = {0, 1, 2, 3};
    renderer->bindIndexData(indices, 4);
    
    renderer->draw();
}
