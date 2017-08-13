#pragma once
#include "../SWidgetComponent.h"

class SSceneComTransform;

class SWidgetComTransform : public SWidgetComponent {
    Q_OBJECT

public:
    SWidgetComTransform(QWidget* parent = nullptr);

    virtual void initialize();

    virtual void refresh();

private:
    SSceneComTransform* _ComTransform;
};
