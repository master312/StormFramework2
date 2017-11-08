#include "SWidgetComponent.h"
#include "componentWidgets/SWidgetComTransform.h"
#include "componentWidgets/SWidgetComScript.h"
#include "../../src/scene/SSceneComponent.h"
#include <QVBoxLayout>
#include <QPainter>
#include <QHBoxLayout>

SWidgetComponent::SWidgetComponent(QWidget* parent) : QWidget(parent) {
    _ComponentName = "ERROR: Name not set!";
    _StormComponent = nullptr;
    _HeaderButton = nullptr;
    _SceneObject = nullptr;
    _BackgroundOpacity = 0.1f;

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
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
    _HeaderButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    connect(_HeaderButton, SIGNAL(clicked(bool)), this, SLOT(collapseButtonClick()));

    _DestroyButtonButton = new QPushButton(this);
    _DestroyButtonButton->setText("X");
    _DestroyButtonButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    connect(_DestroyButtonButton, SIGNAL(clicked(bool)), this, SLOT(destroyComponentClick()));

    if (_StormComponent) {
        _HeaderButton->setText(SSceneComponentTypeString[_StormComponent->getType()].c_str());
    } else {
        _HeaderButton->setText("ERROR: null storm component pointer");
    }

    QHBoxLayout* btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(0);
    btnLayout->setMargin(0);
    btnLayout->addWidget(_HeaderButton);
    btnLayout->addWidget(_DestroyButtonButton);

    layout->addLayout(btnLayout);
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

void SWidgetComponent::destroyComponentClick() {
    parentWidget()->layout()->removeWidget(this);
    delete this;
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
    if (!component) {
        return nullptr;
    }

    /* Component is specified */
    switch (component->getType()) {
        case S_SCENE_OBJECT_COM_SPRITE:
            break;
        case S_SCENE_OBJECT_COM_TRANSFORM:
            widget = new SWidgetComTransform(parent);
            break;
        case S_SCENE_OBJECT_COM_SCRIPT:
            widget = new SWidgetComScript(parent);
            break;
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
