#pragma once
#include "../SWidgetComponent.h"

class SSceneComPlane;

class SWidgetComPlane : public SWidgetComponent {
    Q_OBJECT

public:
    SWidgetComPlane(QWidget* parent = nullptr);
    virtual ~SWidgetComPlane();

    virtual void initialize();

    virtual void refresh();

private:
    SSceneComPlane* _PlaneComponent;
};

