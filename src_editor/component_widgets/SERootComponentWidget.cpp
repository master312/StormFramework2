#include "SERootComponentWidget.h"

SERootComponentWidget::SERootComponentWidget(QWidget* parent) : QWidget(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    _Layout = new QVBoxLayout();
    _Layout->setSpacing(1);
    _Layout->setMargin(0);
    _Layout->setAlignment(Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignTop);

    setMaximumSize(2000, 2000);
    setMinimumSize(180, 20);

    _ToggleButton = new QPushButton(this);
    _ToggleButton->setText("Ovo je sve matrix!");
    _ToggleButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    connect(_ToggleButton, SIGNAL(clicked(bool)), this, SLOT(toggleCollapse()));
    _Layout->addWidget(_ToggleButton);

    setLayout(_Layout);

    /* Set black background */
//    QPalette pallete = palette();
//    pallete.setColor(QPalette::Background, Qt::black);
//    setAutoFillBackground(true);
//    setPalette(pallete);
}

SERootComponentWidget::~SERootComponentWidget() {

}

void SERootComponentWidget::toggleCollapse() {
    /* Collapse (header) button click callback */
    foreach (QWidget* child, findChildren<QWidget*>()) {
        if (child != _ToggleButton) {
            child->setHidden(true);
        }
    }
}

void SERootComponentWidget::addPropertyWidget(SEPropertyWidget* widget) {
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    _Layout->addWidget(widget);
}