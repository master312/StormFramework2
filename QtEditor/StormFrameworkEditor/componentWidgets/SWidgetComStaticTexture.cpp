#include "SWidgetComStaticTexture.h"
#include "../../src/StormEngine.h"
#include "../../src/StormTextureManager.h"
#include "../../src/scene/components/SSceneComStaticTexture.h"
#include "../propertyWidgets/SWidgetPropertyFloat.h"
#include "../propertyWidgets/SWidgetPropertyResource.h"
#include <QLayout>

SWidgetComStaticTexture::SWidgetComStaticTexture(QWidget* parent) : SWidgetComponent(parent) {
    _TextureComponent = nullptr;
    _ResourceSelectWidget = nullptr;
    _ColorOverlay.setZero();
}

void SWidgetComStaticTexture::initialize() {
    SWidgetComponent::initialize();
    _TextureComponent = dynamic_cast<SSceneComStaticTexture*>(_StormComponent);
    if (!_StormComponent) {
        LOG(ERROR) << "SWidgetComStaticTexture::initialize null static texture component";
        return;
    }


    /* Temporary color overlay fix, until color property widget is implemented */
    Color colorOverlay = _TextureComponent->getColorOverlay();
    _ColorOverlay.set((float)colorOverlay.r, (float)colorOverlay.g,
                      (float)colorOverlay.b, (float)colorOverlay.a);
    SWidgetPropertyFloat* redWidget = new SWidgetPropertyFloat(this, "R");
    SWidgetPropertyFloat* greenWidget = new SWidgetPropertyFloat(this, "G");
    SWidgetPropertyFloat* blueWidget = new SWidgetPropertyFloat(this, "B");
    SWidgetPropertyFloat* alphaWidget = new SWidgetPropertyFloat(this, "A");
    _ResourceSelectWidget = new SWidgetPropertyResource(this, "Texture");
    redWidget->setDragFactor(1.0f);
    greenWidget->setDragFactor(1.0f);
    blueWidget->setDragFactor(1.0f);
    alphaWidget->setDragFactor(1.0f);
    redWidget->setValuePointer(&_ColorOverlay.x);
    greenWidget->setValuePointer(&_ColorOverlay.y);
    blueWidget->setValuePointer(&_ColorOverlay.z);
    alphaWidget->setValuePointer(&_ColorOverlay.w);
    layout()->addWidget(redWidget);
    layout()->addWidget(greenWidget);
    layout()->addWidget(blueWidget);
    layout()->addWidget(alphaWidget);
    layout()->addWidget(_ResourceSelectWidget);

    connect(_ResourceSelectWidget, SIGNAL(resourceChanged()), this, SLOT(resourceChanged()));

    foreach (SWidgetProperty* child, findChildren<SWidgetProperty*>()) {
        child->refresh();
    }
}

void SWidgetComStaticTexture::refresh() {
    _TextureComponent->setColorOverlay(Color((uint8_t)_ColorOverlay.x, (uint8_t)_ColorOverlay.y,
                                             (uint8_t)_ColorOverlay.z, (uint8_t)_ColorOverlay.w));
}

void SWidgetComStaticTexture::resourceChanged() {
    std::string filename = _ResourceSelectWidget->getResourceFilename();
    if (!filename.size()) {
        /* No resource selected */
        _TextureComponent->setTexture(nullptr);
        return;
    }

    spStormTexture texture = StormEngine::instance()->getTextureManager()->getTexture(filename);
    _TextureComponent->setTexture(texture);
}
