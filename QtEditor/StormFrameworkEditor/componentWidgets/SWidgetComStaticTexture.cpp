#include "SWidgetComStaticTexture.h"
#include "../../src/StormEngine.h"
#include "../../src/StormTextureManager.h"
#include "../../src/scene/components/SSceneComSprite.h"
#include "../propertyWidgets/SWidgetPropertyFloat.h"
#include "../propertyWidgets/SWidgetPropertyResource.h"
#include "../propertyWidgets/SWidgetPropertyColor.h"
#include <QLayout>

SWidgetComStaticTexture::SWidgetComStaticTexture(QWidget* parent) : SWidgetComponent(parent) {
    _TextureComponent = nullptr;
    _ResourceSelectWidget = nullptr;
    _AlphaControll = nullptr;
    _CurrentAlpha = 0.0f;
    _ColorOverlay.setZero();
}

void SWidgetComStaticTexture::initialize() {
    SWidgetComponent::initialize();
    _TextureComponent = dynamic_cast<SSceneComSprite*>(_StormComponent);
    if (!_StormComponent) {
        LOG(ERROR) << "SWidgetComStaticTexture::initialize null static texture component";
        return;
    }


    /* Temporary color overlay fix, until color property widget is implemented */
    Color colorOverlay = _TextureComponent->getColorMultiply();
    _ColorOverlay.set((float)colorOverlay.r, (float)colorOverlay.g,
                      (float)colorOverlay.b, (float)colorOverlay.a);

    _ResourceSelectWidget = new SWidgetPropertyResource(this, "Texture");
    _AlphaControll = new SWidgetPropertyFloat(this, "Alpha");
    _CurrentAlpha = (float)_TextureComponent->getColorMultiplyPtr()->a;
    _AlphaControll->setValuePointer(&_CurrentAlpha);

    SWidgetPropertyColor* multiplyColorWidget = new SWidgetPropertyColor(this, "Multiply color");
    multiplyColorWidget->setValuePointer(_TextureComponent->getColorMultiplyPtr());

    SWidgetPropertyColor* addColorWidget = new SWidgetPropertyColor(this, "Add color");
    addColorWidget->setValuePointer(_TextureComponent->getColorAddPtr());

    layout()->addWidget(multiplyColorWidget);
    layout()->addWidget(addColorWidget);
    layout()->addWidget(_AlphaControll);
    layout()->addWidget(_ResourceSelectWidget);

    if (_TextureComponent->getTexture()) {
        _ResourceSelectWidget->setResource(StormEngine::getResource(_TextureComponent->getTexture()->getName()));
    }


    connect(_ResourceSelectWidget, SIGNAL(resourceChanged()), this, SLOT(resourceChanged()));

    foreach (SWidgetProperty* child, findChildren<SWidgetProperty*>()) {
        child->refresh();
    }
}

void SWidgetComStaticTexture::refresh() {
    _TextureComponent->setColorMultiply(Color((uint8_t)_ColorOverlay.x, (uint8_t)_ColorOverlay.y,
                                             (uint8_t)_ColorOverlay.z, (uint8_t)_CurrentAlpha));

    foreach (SWidgetProperty* child, findChildren<SWidgetProperty*>()) {
        child->refresh();
    }
}

void SWidgetComStaticTexture::resourceChanged() {
    std::string filename = _ResourceSelectWidget->getResourceFilename();
    if (!filename.size()) {
        /* No resource selected */
        _TextureComponent->setTexture(nullptr);
        return;
    }

    spStormTexture texture = StormEngine::getModule<StormTextureManager>()->getTexture(filename);
    _TextureComponent->setTexture(texture);
}
