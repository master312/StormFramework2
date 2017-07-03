#include "StormObjComPropertyWidget.h"
#include "StormObjComponentWidget.h"
#include <QPainter>

StormObjComPropertyWidget::StormObjComPropertyWidget(StormObjComponentWidget* parent, const std::string& name) : QWidget(parent) {
    _Name = name;
    _ComponentWidgetParent = parent;
    _BackgroundOpacity = 0.15f;
}

StormObjComPropertyWidget::~StormObjComPropertyWidget() {

}

void StormObjComPropertyWidget::refresh() {
}

void StormObjComPropertyWidget::enterEvent(QEvent* event) {
    _BackgroundOpacity = 0.4f;
    QWidget::enterEvent(event);
    repaint();
}

void StormObjComPropertyWidget::leaveEvent(QEvent* event) {
    _BackgroundOpacity = 0.15f;
    QWidget::leaveEvent(event);
    repaint();
}

void StormObjComPropertyWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    painter.setOpacity(_BackgroundOpacity);
    painter.fillRect(0, 0, width(), height(), Qt::BrushStyle::SolidPattern);

    QWidget::paintEvent(event);
}

QLabel* StormObjComPropertyWidget::generateNameLabel() {
    QLabel* label = new QLabel(this);
    label->setText(std::string(_Name + ": ").c_str());

    return label;
}
