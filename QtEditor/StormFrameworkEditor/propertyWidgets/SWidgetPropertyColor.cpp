#include "SWidgetPropertyColor.h"
#include "../../src/core/StormCommon.h"
#include "../SNumberLineEdit.h"
#include <QBoxLayout>
#include <QColorDialog>
#include <QPalette>

SWidgetPropertyColor::SWidgetPropertyColor(SWidgetComponent* parent, const std::string& name) : SWidgetProperty(parent, name) {
    _Color = nullptr;

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(1);

    _ColorPriview = new QLineEdit(this);
    _ColorPriview->setEnabled(false);

    layout->addWidget(generateNameLabel());
    layout->addWidget(_ColorPriview);

    setLayout(layout);
}

void SWidgetPropertyColor::refresh() {
    if (!_Color) {
        return;
    }
    QPalette palette;
    palette.setColor(QPalette::Base, QColor(_Color->r, _Color->g, _Color->b));
    _ColorPriview->setPalette(palette);
}

void SWidgetPropertyColor::setValuePointer(Color* color) {
    _Color = color;
}

void SWidgetPropertyColor::mousePressEvent(QMouseEvent* event) {
    QColor color = QColorDialog::getColor(Qt::yellow, this);
    if (!color.isValid()) {
        LOG(ERROR) << "SWidgetPropertyColor Invalid color chosen";
        return;
    }
    if (_Color) {
        _Color->r = (uint8_t)color.red();
        _Color->g = (uint8_t)color.green();
        _Color->b = (uint8_t)color.blue();
        refresh();
    }
}
