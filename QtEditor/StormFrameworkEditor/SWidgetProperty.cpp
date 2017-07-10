#include "SWidgetProperty.h"
#include "SWidgetComponent.h"
#include <QPainter>

SWidgetProperty::SWidgetProperty(SWidgetComponent* parent, const std::string& name) : QWidget(parent) {
    _Name = name;
    _ComponentWidgetParent = parent;
    _BackgroundOpacity = 0.15f;
}

SWidgetProperty::~SWidgetProperty() {

}

void SWidgetProperty::refresh() {
}

void SWidgetProperty::enterEvent(QEvent* event) {
    _BackgroundOpacity = 0.45f;
    QWidget::enterEvent(event);
    repaint();
}

void SWidgetProperty::leaveEvent(QEvent* event) {
    _BackgroundOpacity = 0.15f;
    QWidget::leaveEvent(event);
    repaint();
}

void SWidgetProperty::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    painter.setOpacity(_BackgroundOpacity);
    painter.fillRect(0, 0, width(), height(), Qt::BrushStyle::SolidPattern);

    QWidget::paintEvent(event);
}

QLabel* SWidgetProperty::generateNameLabel() {
    QLabel* label = new QLabel(this);
    label->setText(std::string(_Name + ": ").c_str());

    return label;
}
