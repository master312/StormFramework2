#include "StormPropertyVec2Widget.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSpacerItem>
#include <QDoubleValidator>
#include <QMouseEvent>
#include "../StormObjComponentWidget.h"
#include "../../src/core/StormCommon.h"

StormDragLineEdit::StormDragLineEdit(QWidget* parent) : QLineEdit(parent) {

}

void StormDragLineEdit::mousePressEvent(QMouseEvent* event) {
    _IsDragging = true;
    _DragStartPosition = event->pos();
}

void StormDragLineEdit::mouseReleaseEvent(QMouseEvent* event) {
    _IsDragging = false;
    editingFinished();
}

void StormDragLineEdit::mouseMoveEvent(QMouseEvent* event){
    if (!_IsDragging) {
        return;
    }
    bool ok = true;
    float number = text().toFloat(&ok);
    if (!ok) {
        return;
    }
    float dist = (float)(_DragStartPosition.x() - event->pos().x()) / 2.0f;
    number -= (float)dist;

    setText(QString::number(number));
    _DragStartPosition = event->pos();
    editingFinished();
}




StormPropertyVec2Widget::StormPropertyVec2Widget(StormObjComponentWidget* parent, const std::string& name) : StormObjComPropertyWidget(parent, name) {
    _VectorSetter = nullptr;
    _VectorFloatSetter = nullptr;
    _VectorGetter = nullptr;

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(1);

    _XPosEdit = new StormDragLineEdit(this);
    _XPosEdit->setValidator(new QDoubleValidator());
    _YPosEdit = new StormDragLineEdit(this);
    _YPosEdit->setValidator(new QDoubleValidator());

    layout->addWidget(generateNameLabel());
    layout->addWidget(new QLabel("X:", this));
    layout->addWidget(_XPosEdit);
    layout->addWidget(new QLabel("Y:", this));
    layout->addWidget(_YPosEdit);

    connect(_XPosEdit, SIGNAL(editingFinished()), this, SLOT(editingFinished()));
    connect(_YPosEdit, SIGNAL(editingFinished()), this, SLOT(editingFinished()));

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
