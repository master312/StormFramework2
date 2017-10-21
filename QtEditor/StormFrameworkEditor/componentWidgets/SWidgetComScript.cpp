#include "SWidgetComScript.h"
#include "../../src/core/StormCommon.h"
#include "../../src/StormEngine.h"
#include "../../src/scene/components/SSceneComLuaScript.h"
#include "../propertyWidgets/SWidgetPropertyResource.h"
#include <QVBoxLayout>

SWidgetComScript::SWidgetComScript(QWidget* parent /* = nullptr */) : SWidgetComponent(parent) {
    _ScriptComponent = nullptr;
    _ResourceSelectWidget = nullptr;
}

SWidgetComScript::~SWidgetComScript() {

}

void SWidgetComScript::initialize() {
    SWidgetComponent::initialize();
    _ScriptComponent = dynamic_cast<SSceneComLuaScript*>(_StormComponent);
    if (!_ScriptComponent) {
        LOG(ERROR) << "SWidgetComScript::initialize null script component";
        return;
    }

    _ResourceSelectWidget = new SWidgetPropertyResource(this, "Script file");
    layout()->addWidget(_ResourceSelectWidget);

    if (_ScriptComponent->getFilename().size() > 0) {
        _ResourceSelectWidget->setResource(StormEngine::getResource(_ScriptComponent->getFilename()));
    }
}

void SWidgetComScript::refresh() {
}
