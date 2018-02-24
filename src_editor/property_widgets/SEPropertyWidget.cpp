#include "SEPropertyWidget.h"
#include <QPainter>

SEPropertyWidget::SEPropertyWidget(QWidget* parent, const std::string& name /* = "Property widget" */) : QWidget(parent) {
    _Name = name;
    _BackgroundOpacity = 0.15f;

    _Layout = generateVBoxLayout();
    //_Layout->addWidget(generateLabel(_Name.c_str(), this));

    setMinimumSize(parent ? parent->minimumWidth() - 5 : 100, 15);
    setMaximumSize(parent ? parent->maximumWidth() - 5 : 100, 200);
}

SEPropertyWidget::~SEPropertyWidget() {
}

void SEPropertyWidget::refreshValues() {
    refresh();
}

void SEPropertyWidget::refresh() {
}

void SEPropertyWidget::enterEvent(QEvent* event) {
    _BackgroundOpacity = 0.45f;
    QWidget::enterEvent(event);
    repaint();
}

void SEPropertyWidget::leaveEvent(QEvent* event) {
    _BackgroundOpacity = 0.15f;
    QWidget::leaveEvent(event);
    repaint();
}

void SEPropertyWidget::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    painter.setOpacity(_BackgroundOpacity);
    painter.fillRect(0, 0, width(), height(), Qt::BrushStyle::SolidPattern);

    QWidget::paintEvent(event);
}

QLayout* SEPropertyWidget::generateVBoxLayout() {
    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(1);
    layout->setMargin(0);
    layout->setAlignment(Qt::AlignmentFlag::AlignTop);
    return layout;
}

QLabel* SEPropertyWidget::generateLabel(const QString& text,
                                        QWidget* parent /* = nullptr*/ ) {
    QLabel* label = new QLabel(parent);
    label->setText(text + ": ");
    return label;
}