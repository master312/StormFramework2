#include "SSceneComStaticTexture.h"
#include "SSceneComPlane.h"
#include "../StormSceneObject.h"
#include "../../StormEngine.h"

SSceneComStaticTexture::SSceneComStaticTexture(StormSceneObject* owner) : SSceneComponent(owner) {
    _PlaneComponent = nullptr;
    _Texture = nullptr;
    _ColorOverlay.set(255, 255, 255, 255);
    
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
    node.append_attribute("color_r").set_value((int)_ColorOverlay.r);
    node.append_attribute("color_g").set_value((int)_ColorOverlay.g);
    node.append_attribute("color_b").set_value((int)_ColorOverlay.b);
    node.append_attribute("color_a").set_value((int)_ColorOverlay.a);
}

int SSceneComStaticTexture::deserializeXml(pugi::xml_node& node) {
    SSceneComponent::deserializeXml(node);

    std::string textureName = node.attribute("texture").as_string("");
    if (textureName == "") {
        LOG(WARNING) << "Texture name not specified in XML file for SSceneComStaticTexture. Object ID " << getOwner()->getId();
    }
    _Texture = StormEngine::instance()->getTextureManager()->getTexture(textureName);

    _ColorOverlay.r = (uint8_t)node.attribute("color_r").as_int(255);
    _ColorOverlay.g = (uint8_t)node.attribute("color_g").as_int(255);
    _ColorOverlay.b = (uint8_t)node.attribute("color_b").as_int(255);
    _ColorOverlay.a = (uint8_t)node.attribute("color_a").as_int(255);

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

void SSceneComStaticTexture::setColorOverlay(Color color) {
    _ColorOverlay = color;
}

Color SSceneComStaticTexture::getColorOverlay() {
    return _ColorOverlay;
}

void SSceneComStaticTexture::render(StormRenderer* renderer) {
    renderer->begin(S_RENDER_TRIANGLE_FAN);
    renderer->bindTexture(_Texture.get());
    renderer->setColorMultiply(_ColorOverlay);
    renderer->bindVertexData(_PlaneComponent->getVertices(), 4);
    
    uint32_t indices[] = {0, 1, 2, 3};
    renderer->bindIndexData(indices, 4);
    
    renderer->draw();
}
