#include "SEComponentWidget.h"
#include <QVBoxLayout>
#include <QPalette>

SEComponentWidget::SEComponentWidget(QWidget* parent) : QWidget(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->setSpacing(1);
    layout->setMargin(0);
    layout->setAlignment(Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignTop);

    setMaximumSize(1800, 1800);
    setMinimumSize(300, 50);

    _ToggleButton = new QPushButton(this);
    _ToggleButton->setText("Ovo je sve matrix!");
    _ToggleButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    layout->addWidget(_ToggleButton);

    setLayout(layout);
    
    /* Set black background */
    QPalette pallete = palette();
    pallete.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pallete);
}

SEComponentWidget::~SEComponentWidget() {

}