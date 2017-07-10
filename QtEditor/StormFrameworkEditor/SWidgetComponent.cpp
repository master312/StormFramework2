#include "SWidgetComponent.h"
#include "componentWidgets/SWidgetComPlane.h"
#include "componentWidgets/SWidgetComStaticTexture.h"
#include <QVBoxLayout>
#include <QPainter>

SWidgetComponent::SWidgetComponent(QWidget* parent) : QWidget(parent) {
    _ComponentName = "ERROR: Name not set!";
    _StormComponent = nullptr;
    _HeaderButton = nullptr;
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
SWidgetComponent* SWidgetComponent::newWidget(SSceneComponentType type, SSceneComponent* component, QWidget* parent) {
    SWidgetComponent* widget = nullptr;
    switch (type) {
        case S_SCENE_OBJECT_COM_PLANE:
            widget = new SWidgetComPlane(parent);
            break;
        case S_SCENE_OBJECT_COM_STATIC_TEXTURE:
            widget = new SWidgetComStaticTexture(parent);
            break;
    }
    if (widget) {
        widget->setStormComponent(component);
    }

    return widget;
}
