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
    _IsDragging = false;
    _DragStartPosition.setX(0);
    _DragStartPosition.setY(0);
    _DragVariableFactor.set(1.0f, 1.0f);

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

void StormPropertyVec2Widget::valuesChanged() {
    if (_VectorSetter) {
        _VectorSetter(Vector2(_XPosEdit->text().toFloat(), _YPosEdit->text().toFloat()));
    } else if (_VectorFloatSetter) {
        _VectorFloatSetter(_XPosEdit->text().toFloat(), _YPosEdit->text().toFloat());
    }

    _ComponentWidgetParent->refresh();
}

void StormPropertyVec2Widget::mousePressEvent(QMouseEvent* event) {
    _IsDragging = true;
    _DragStartPosition = event->pos();
    QApplication::setOverrideCursor(Qt::SizeAllCursor);
}

void StormPropertyVec2Widget::mouseReleaseEvent(QMouseEvent* event) {
    _IsDragging = false;
    QApplication::restoreOverrideCursor();
}

void StormPropertyVec2Widget::mouseMoveEvent(QMouseEvent* event) {
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

void StormPropertyVec2Widget::readValues() {
    if (!_VectorGetter) {
        return;
    }
    Vector2 vec = _VectorGetter();
    _XPosEdit->setText(QString::number(vec.x));
    _YPosEdit->setText(QString::number(vec.y));
}
