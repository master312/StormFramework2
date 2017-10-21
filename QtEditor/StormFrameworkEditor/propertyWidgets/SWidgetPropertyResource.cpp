#include "SWidgetPropertyResource.h"
#include <QHBoxLayout>
#include "../SResourceLineEdit.h"
#include "../../src/StormEngine.h"
#include "../../src/core/resources/StormFileSystem.h"

SWidgetPropertyResource::SWidgetPropertyResource(SWidgetComponent* parent, const std::string& name) : SWidgetProperty(parent, name) {
    _ResourceFile = nullptr;

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(1);

    _ResourceLineEdit = new SResourceLineEdit(this);

    layout->addWidget(generateNameLabel());
    layout->addWidget(_ResourceLineEdit);

    setLayout(layout);

    connect(_ResourceLineEdit, SIGNAL(resourceSelected()), this, SLOT(resourceSelected()));
}

void SWidgetPropertyResource::setResource(spStormResourceFile resource) {
    _ResourceFile = resource;
    if (_ResourceFile) {
        _ResourceLineEdit->setText(_ResourceFile->getFilenameWithExt().c_str());
    } else {
        _ResourceLineEdit->setText("");
    }

    resourceChanged();
}

spStormResourceFile SWidgetPropertyResource::getResource() {
    return _ResourceFile;
}

std::string SWidgetPropertyResource::getResourceFilename() const {
    if (_ResourceFile) {
        return _ResourceLineEdit->text().toStdString();
    }
    return "";
}

void SWidgetPropertyResource::resourceSelected() {
    QString resourceFilename = _ResourceLineEdit->text();
    if (!resourceFilename.size()) {
        /* No resource selected */
        return;
    }

    _ResourceFile = StormEngine::getModule<StormFileSystem>()->
                                             getResourceByFilename(resourceFilename.toStdString());
    if (!_ResourceFile) {
        /* TODO: Maybe log error ? */
        return;
    }

    resourceChanged();
}
