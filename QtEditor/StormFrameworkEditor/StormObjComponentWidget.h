#pragma once
#include <QWidget>
#include <QPushButton>
#include "../../src/scene/SSceneComponent.h"

/* Base class for all storm object component widgets.
 * Make sure that StormObjComponentWidget objects are created using StormObjComponentWidget::newWidget method. */

class StormObjComponentWidget : public QWidget {
    Q_OBJECT

public:
    StormObjComponentWidget(QWidget* parent = nullptr);
    virtual ~StormObjComponentWidget();

    void setStormComponent(SSceneComponent* component);
    virtual void initialize();

    /* Ussualy called from Property widgets when values ware edited, to update storm component */
    virtual void refresh();

    /* Component widget factory method */
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
