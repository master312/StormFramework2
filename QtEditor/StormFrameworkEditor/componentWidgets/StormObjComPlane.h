#pragma once
#include <QWidget>
#include "../StormObjComponentWidget.h"
#include "../../src/scene/components/SSceneComPlane.h"

class StormObjComPlane : public StormObjComponentWidget {
public:
    StormObjComPlane(QWidget* parent = nullptr);

    virtual void initialize();

    virtual void refresh();
private:
    SSceneComPlane* _PlaneComponent;
};

