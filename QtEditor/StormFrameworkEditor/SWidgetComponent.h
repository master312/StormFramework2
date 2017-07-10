#pragma once
#include <QWidget>
#include <QPushButton>
#include "../../src/scene/SSceneComponent.h"

/* Base class for all storm object component widgets.
 * Make sure that StormObjComponentWidget objects are created using StormObjComponentWidget::newWidget method. */

class SWidgetComponent : public QWidget {
    Q_OBJECT

public:
    SWidgetComponent(QWidget* parent = nullptr);
    virtual ~SWidgetComponent();

    void setStormComponent(SSceneComponent* component);
    virtual void initialize();

    /* Usualy called from Property widgets when values ware edited, to update storm component */
    virtual void refresh();

    /* Component widget factory method */
    static SWidgetComponent* newWidget(SSceneComponentType type, SSceneComponent* component, QWidget* parent);

public slots:
    void collapseButtonClick();

protected:
    virtual void enterEvent(QEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void paintEvent(QPaintEvent* event);

    /* Component's name */
    std::string _ComponentName;

    /* Pointer to storm scene object's component */
    SSceneComponent* _StormComponent;

    float _BackgroundOpacity;
    QPushButton* _HeaderButton;
};
