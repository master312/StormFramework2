#include "SSceneSystemStaticTexture.h"
#include "SSceneComStaticTexture.h"
#include "SSceneComPlane.h"
#include "../StormSceneObject.h"

SSceneSystemStaticTexture::SSceneSystemStaticTexture() {
    _Type = S_SCENE_OBJECT_COM_SYS_STATIC_TEXTURE;
}

SSceneSystemStaticTexture::~SSceneSystemStaticTexture() {
}

void SSceneSystemStaticTexture::addComponent(SSceneComponent* component) {
    if (!validateComponent(component, S_SCENE_OBJECT_COM_STATIC_TEXTURE)) {
        return;
    }
    SSceneComponentSystem::addComponent(component);

    SSceneComStaticTexture* com = dynamic_cast<SSceneComStaticTexture*>(component);
    SSceneComPlane* planeCom = dynamic_cast<SSceneComPlane*>(com->getOwner()->getComponent(S_SCENE_OBJECT_COM_PLANE));
    if (!planeCom) {
        LOG(ERROR) << "Could not add StaticTextureComponent. Owner dose not have a plane.";
        return;
    }
    com->setPlaneComponent(planeCom);

    _TextureComponents.push_back(com);
}

void SSceneSystemStaticTexture::render(StormRenderer* renderer) {
    for (size_t i = 0; i < _TextureComponents.size(); i++) {
        _TextureComponents[i]->render(renderer);
    }
}