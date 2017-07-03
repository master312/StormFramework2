#include "StormPropertyVec2Widget.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSpacerItem>
#include <QMouseEvent>
#include "../StormObjComponentWidget.h"
#include "../../src/core/StormCommon.h"

StormPropertyVec2Widget::StormPropertyVec2Widget(StormObjComponentWidget* parent, const std::string& name) : StormObjComPropertyWidget(parent, name) {
    _VectorSetter = nullptr;
    _VectorFloatSetter = nullptr;
    _VectorGetter = nullptr;

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(1);

    _XPosEdit = new SNumberLineEdit(this);
    _YPosEdit = new SNumberLineEdit(this);

    layout->addWidget(generateNameLabel());
    layout->addWidget(new QLabel("X:", this));
    layout->addWidget(_XPosEdit);
    layout->addWidget(new QLabel("Y:", this));
    layout->addWidget(_YPosEdit);

    connect(_XPosEdit, SIGNAL(valueChanged()), this, SLOT(editingFinished()));
    connect(_YPosEdit, SIGNAL(valueChanged()), this, SLOT(editingFinished()));

    setLayout(layout);
}

StormPropertyVec2Widget::~StormPropertyVec2Widget() {

}

void StormPropertyVec2Widget::refresh() {
    readValues();
}

void StormPropertyVec2Widget::setVector(Vector2* vector) {
    _VectorFloatSetter = std::bind(&Vector2::set, vector, std::placeholders::_1, std::placeholders::_2);
    _VectorGetter = std::bind(&Vector2::get, vector);
}

void StormPropertyVec2Widget::setVectorSetter(std::function<void(float, float)> setter) {
    _VectorFloatSetter = setter;
}

void StormPropertyVec2Widget::setVectorSetter(std::function<void(Vector2)> setter) {
    _VectorSetter = setter;
}

void StormPropertyVec2Widget::setVectorGetter(std::function<Vector2()> getter) {
    _VectorGetter = getter;
}

void StormPropertyVec2Widget::editingFinished() {
    if (_VectorSetter) {
        _VectorSetter(Vector2(_XPosEdit->text().toFloat(), _YPosEdit->text().toFloat()));
        _ComponentWidgetParent->refresh();
        return;
    }
}

void StormPropertyVec2Widget::readValues() {
    if (!_VectorGetter) {
        return;
    }
    Vector2 vec = _VectorGetter();
    _XPosEdit->setText(QString::number(vec.x));
    _YPosEdit->setText(QString::number(vec.y));
}
