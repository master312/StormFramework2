#pragma once
#include <QWidget>
#include <QPushButton>
#include "../../src/scene/SSceneComponent.h"

/* Base class for all storm object component widgets */

class StormObjComponentWidget : public QWidget {
    Q_OBJECT

public:
    StormObjComponentWidget(QWidget* parent = 0);
    virtual ~StormObjComponentWidget();

    void setStormComponent(SSceneComponent* component);
    void initialize();

    static StormObjComponentWidget* newWidget(SSceneComponentType type, SSceneComponent* component);

public slots:
    void collapseButtonClick();

protected:
    /* Component's name */
    std::string _ComponentName;

    /* Pointer to storm scene object's component */
    SSceneComponent* _StormComponent;

    QPushButton* _HeaderButton;
};
