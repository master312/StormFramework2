#include "SWidgetPropertyVec2.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSpacerItem>
#include <QMouseEvent>
#include "../SWidgetComponent.h"
#include "../../src/core/StormCommon.h"

SWidgetPropertyVec2::SWidgetPropertyVec2(SWidgetComponent* parent, const std::string& name) : SWidgetProperty(parent, name) {
    _VectorSetter = nullptr;
    _VectorFloatSetter = nullptr;
    _VectorGetter = nullptr;
    _IsDragging = false;
    _DragStartPosition.setX(0);
    _DragStartPosition.setY(0);

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

    connect(_XPosEdit, SIGNAL(valueChanged()), this, SLOT(valuesChanged()));
    connect(_YPosEdit, SIGNAL(valueChanged()), this, SLOT(valuesChanged()));
    connect(_XPosEdit, SIGNAL(editingFinished()), this, SLOT(editingFinished()));
    connect(_YPosEdit, SIGNAL(editingFinished()), this, SLOT(editingFinished()));

    setDragFactor(1.0f);
    setLayout(layout);
}

SWidgetPropertyVec2::~SWidgetPropertyVec2() {
}

void SWidgetPropertyVec2::refresh() {
    SWidgetProperty::refresh();

    readValues();
}

void SWidgetPropertyVec2::setVector(Vector2* vector) {
    _VectorFloatSetter = std::bind(static_cast<void(Vector2::*)(float, float)>(&Vector2::set), vector, std::placeholders::_1, std::placeholders::_2);
    _VectorGetter = std::bind(&Vector2::get, vector);
}

void SWidgetPropertyVec2::setVectorSetter(std::function<void(float, float)> setter) {
    _VectorFloatSetter = setter;
}

void SWidgetPropertyVec2::setVectorSetter(std::function<void(Vector2)> setter) {
    _VectorSetter = setter;
}

void SWidgetPropertyVec2::setVectorGetter(std::function<Vector2()> getter) {
    _VectorGetter = getter;
}

void SWidgetPropertyVec2::setDragFactor(float factor) {
    _DragVariableFactor.set(factor, factor);
    _XPosEdit->setDragFactor(factor);
    _YPosEdit->setDragFactor(factor);
}

void SWidgetPropertyVec2::setDragFactor(Vector2 factor) {
    _DragVariableFactor = factor;
    _XPosEdit->setDragFactor(factor.x);
    _YPosEdit->setDragFactor(factor.y);
}

void SWidgetPropertyVec2::setDragFactorX(float factor){
    _DragVariableFactor.x = factor;
    _XPosEdit->setDragFactor(factor);
}

void SWidgetPropertyVec2::setDragFactorY(float factor){
    _DragVariableFactor.y = factor;
    _YPosEdit->setDragFactor(factor);
}

void SWidgetPropertyVec2::valuesChanged() {
    if (_VectorSetter) {
        _VectorSetter(Vector2(_XPosEdit->text().toFloat(), _YPosEdit->text().toFloat()));
    } else if (_VectorFloatSetter) {
        _VectorFloatSetter(_XPosEdit->text().toFloat(), _YPosEdit->text().toFloat());
    }

    _ComponentWidgetParent->refresh();
}

void SWidgetPropertyVec2::editingFinished() {
    refresh();
}

void SWidgetPropertyVec2::mousePressEvent(QMouseEvent* event) {
    _IsDragging = true;
    _DragStartPosition = event->pos();
    QApplication::setOverrideCursor(Qt::SizeAllCursor);
}

void SWidgetPropertyVec2::mouseReleaseEvent(QMouseEvent* event) {
    _IsDragging = false;
    QApplication::restoreOverrideCursor();
}

void SWidgetPropertyVec2::mouseMoveEvent(QMouseEvent* event) {
    if (!_IsDragging) {
        return;
    }
    float changeX = (float)(_DragStartPosition.x() - event->pos().x()) * _DragVariableFactor.x;
    float changeY = (float)(_DragStartPosition.y() - event->pos().y()) * _DragVariableFactor.y;

    if ((changeX < 0.1f && changeX > -0.1f) && (changeY < 0.1f && changeY > -0.1f)){
        return;
    }

    float newX = _XPosEdit->text().toFloat() - changeX;
    float newY = _YPosEdit->text().toFloat() - changeY;

    _XPosEdit->setText(QString::number(newX));
    _YPosEdit->setText(QString::number(newY));

    valuesChanged();

    _DragStartPosition = event->pos();
}

void SWidgetPropertyVec2::readValues() {
    if (!_VectorGetter) {
        return;
    }
    Vector2 vec = _VectorGetter();
    _XPosEdit->setText(QString::number(vec.x));
    _YPosEdit->setText(QString::number(vec.y));
}
