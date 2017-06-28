#include "SSceneSystemStaticTexture.h"
#include "SSceneComStaticTexture.h"
#include "SSceneComPlane.h"
#include "../StormSceneObject.h"

SSceneSystemStaticTexture::SSceneSystemStaticTexture() {
    _Type = S_SCENE_OBJECT_COM_SYS_TEXTURE;
}

SSceneSystemStaticTexture::~SSceneSystemStaticTexture() {
}

void SSceneSystemStaticTexture::addComponent(SSceneComponent* component) {
    if (!validateComponent(component, S_SCENE_OBJECT_COM_TEXTURE)) {
        return;
    }

    SSceneComStaticTexture* com = dynamic_cast<SSceneComStaticTexture*>(component);
    SSceneComPlane* planeCom = dynamic_cast<SSceneComPlane*>(com->getOwner()->getComponent(S_SCENE_OBJECT_COM_PLANE));
    if (!planeCom) {
        LOG(ERROR) << "Could not add StaticTextureComponent. Owner dose not have a plane.";
        return;
    }
    com->setPlaneComponent(planeCom);

    _Components.push_back(com);
}

void SSceneSystemStaticTexture::render(StormRenderer* renderer) {
    for (unsigned int i = 0; i < _Components.size(); i++) {
        _Components[i]->render(renderer);
    }
}