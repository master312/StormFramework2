#include "StormObjComponentWidget.h"
#include "componentWidgets/StormObjComPlane.h"
#include <QVBoxLayout>
#include <QPainter>

StormObjComponentWidget::StormObjComponentWidget(QWidget* parent) : QWidget(parent) {
    _ComponentName = "ERROR: Name not set!";
    _StormComponent = nullptr;
    _HeaderButton = nullptr;
    _BackgroundOpacity = 0.1f;
}

StormObjComponentWidget::~StormObjComponentWidget() {
}

void StormObjComponentWidget::setStormComponent(SSceneComponent* component) {
    _StormComponent = component;
}

void StormObjComponentWidget::initialize() {
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
    layout->addWidget(new QPushButton(this));
    layout->addWidget(new QPushButton(this));
    setLayout(layout);
}

void StormObjComponentWidget::refresh() {
}

void StormObjComponentWidget::collapseButtonClick() {
    /* Collapse (header) button click callback */
    foreach (QWidget* child, findChildren<QWidget*>()) {
        if (child != _HeaderButton) {
            child->setHidden(!child->isHidden());
        }
    }
}

void StormObjComponentWidget::enterEvent(QEvent* event) {
    _BackgroundOpacity = 0.3f;
    QWidget::enterEvent(event);
    repaint();
}

void StormObjComponentWidget::leaveEvent(QEvent* event) {
    _BackgroundOpacity = 0.1f;
    QWidget::leaveEvent(event);
    repaint();
}

void StormObjComponentWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    painter.setOpacity(_BackgroundOpacity);
    painter.fillRect(0, 0, width(), height(), Qt::BrushStyle::SolidPattern);

    QWidget::paintEvent(event);
}

/* Static factory method */
StormObjComponentWidget* StormObjComponentWidget::newWidget(SSceneComponentType type, SSceneComponent* component) {
    StormObjComponentWidget* widget = nullptr;
    switch (type) {
        case S_SCENE_OBJECT_COM_PLANE:
            widget = new StormObjComPlane();
            break;

    }
    if (widget) {
        widget->setStormComponent(component);
    }

    return widget;
}
