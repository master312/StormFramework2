#include "SWidgetPropertyResource.h"
#include <QHBoxLayout>
#include "../../src/StormEngine.h"

SWidgetPropertyResource::SWidgetPropertyResource(SWidgetComponent* parent, const std::string& name) : SWidgetProperty(parent, name) {
    _ResourceFile = nullptr;

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(1);

    layout->addWidget(generateNameLabel());

    setLayout(layout);
}

void SWidgetPropertyResource::setResource(spStormResourceFile resource) {
    _ResourceFile = resource;
}

spStormResourceFile SWidgetPropertyResource::getResource() {
    return _ResourceFile;
}


