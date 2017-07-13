#include "SWidgetComponent.h"
#include "componentWidgets/SWidgetComDefault.h"
#include "componentWidgets/SWidgetComPlane.h"
#include "componentWidgets/SWidgetComStaticTexture.h"
#include "../../src/scene/SSceneComponent.h"
#include <QVBoxLayout>
#include <QPainter>

SWidgetComponent::SWidgetComponent(QWidget* parent) : QWidget(parent) {
    _ComponentName = "ERROR: Name not set!";
    _StormComponent = nullptr;
    _HeaderButton = nullptr;
    _SceneObject = nullptr;
    _BackgroundOpacity = 0.1f;
}

SWidgetComponent::~SWidgetComponent() {
}

void SWidgetComponent::setStormComponent(SSceneComponent* component) {
    _StormComponent = component;
}

void SWidgetComponent::initialize() {
    QVBoxLayout* layout = new QVBoxLayout;
    layout->setSpacing(1);
    layout->setMargin(0);
    layout->setAlignment(Qt::AlignmentFlag::AlignTop);

    _HeaderButton = new QPushButton(this);
    connect(_HeaderButton, SIGNAL(clicked(bool)), this, SLOT(collapseButtonClick()));

    if (_StormComponent) {
        _HeaderButton->setText(SSceneComponentTypeString[_StormComponent->getType()].c_str());
    } else {
        _HeaderButton->setText("ERROR: null storm component pointer");
    }

    layout->addWidget(_HeaderButton);
    setLayout(layout);
}

void SWidgetComponent::refresh() {
}

StormSceneObject* SWidgetComponent::getSceneObject() {
    return _SceneObject;
}

void SWidgetComponent::collapseButtonClick() {
    /* Collapse (header) button click callback */
    foreach (QWidget* child, findChildren<QWidget*>()) {
        if (child != _HeaderButton) {
            child->setHidden(!child->isHidden());
        }
    }
}

void SWidgetComponent::enterEvent(QEvent* event) {
    _BackgroundOpacity = 0.3f;
    QWidget::enterEvent(event);
    repaint();
}

void SWidgetComponent::leaveEvent(QEvent* event) {
    _BackgroundOpacity = 0.1f;
    QWidget::leaveEvent(event);
    repaint();
}

void SWidgetComponent::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    painter.setOpacity(_BackgroundOpacity);
    painter.fillRect(0, 0, width(), height(), Qt::BrushStyle::SolidPattern);

    QWidget::paintEvent(event);
}

/* Static factory method */
SWidgetComponent* SWidgetComponent::newWidget(StormSceneObject* object, SSceneComponent* component, QWidget* parent) {
    SWidgetComponent* widget = nullptr;
    if (component) {
        /* Component is specified */
        switch (component->getType()) {
            case S_SCENE_OBJECT_COM_PLANE:
                widget = new SWidgetComPlane(parent);
                break;
            case S_SCENE_OBJECT_COM_STATIC_TEXTURE:
                widget = new SWidgetComStaticTexture(parent);
                break;
        }
    } else {
        /* Component is not specified. Generate default widget */
        widget = new SWidgetComDefault(parent);
    }

    if (!widget) {
        return nullptr;
    }

    if (component) {
        widget->setStormComponent(component);
    }
    if (object) {
        widget->_SceneObject = object;
    }

    return widget;
}
