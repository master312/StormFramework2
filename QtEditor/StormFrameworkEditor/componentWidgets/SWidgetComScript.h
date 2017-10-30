#pragma once
#include "../SWidgetComponent.h"

class SSceneComLuaScript;
class SWidgetPropertyResource;

class SWidgetComScript : public SWidgetComponent {
    Q_OBJECT

public:
    SWidgetComScript(QWidget* parent = nullptr);
    virtual ~SWidgetComScript();

    virtual void initialize();

    virtual void refresh();

private:
    SSceneComLuaScript* _ScriptComponent;
    SWidgetPropertyResource* _ResourceSelectWidget;
};

