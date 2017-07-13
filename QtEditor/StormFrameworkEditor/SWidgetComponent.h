#pragma once
#include <QWidget>
#include <QPushButton>

class SSceneComponent;
class StormSceneObject;

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

    StormSceneObject* getSceneObject();

    /* Component widget factory method.
     * Method will generate SWidgetComDefault if @component == nullptr. */
    static SWidgetComponent* newWidget(StormSceneObject* object, SSceneComponent* component, QWidget* parent);

public slots:
    void collapseButtonClick();

protected:
    virtual void enterEvent(QEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void paintEvent(QPaintEvent* event);

    /* Component's name */
    std::string _ComponentName;

    /* Pointer to actual storm scene object */
    StormSceneObject* _SceneObject;

    /* Pointer to storm scene object's component */
    SSceneComponent* _StormComponent;

    float _BackgroundOpacity;
    QPushButton* _HeaderButton;
};
