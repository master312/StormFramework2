#include <QApplication>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMouseEvent>
#include "SEPropertyVector2.h"
#include "../../src/core/StormCommon.h"

SEPropertyVector2::SEPropertyVector2(QWidget* parent, const std::string& name) : SEPropertyWidget(parent, name) {
    _IsDragging = false;
    _DragStartPosition.setX(0);
    _DragStartPosition.setY(0);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(1);

    _XPosEdit = new SENumberLineEdit(this);
    _YPosEdit = new SENumberLineEdit(this);

    layout->addWidget(generateLabel(name.c_str()));
    layout->addWidget(new QLabel("X:", this));
    layout->addWidget(_XPosEdit);
    layout->addWidget(new QLabel("Y:", this));
    layout->addWidget(_YPosEdit);

    connect(_XPosEdit, SIGNAL(valueChanged()), this, SLOT(valuesChanged()));
    connect(_YPosEdit, SIGNAL(valueChanged()), this, SLOT(valuesChanged()));
    connect(_XPosEdit, SIGNAL(editingFinished()), this, SLOT(editingFinished()));
    connect(_YPosEdit, SIGNAL(editingFinished()), this, SLOT(editingFinished()));

    setDragFactor(0.005f);
    setLayout(layout);
}

SEPropertyVector2::~SEPropertyVector2() {
}

void SEPropertyVector2::setValue(const Vector2& vec) {
    _Value.set(vec);
    refresh();
}

void SEPropertyVector2::setValueX(const float x) {
    _Value.x = x;
    refresh();
}

void SEPropertyVector2::setValueY(const float y) {
    _Value.y = y;
    refresh();
}

SEPropertyVector2* SEPropertyVector2::create(QWidget* parent, const std::string& name) {
    SEPropertyVector2* widget = new SEPropertyVector2(parent, name);
    if (parent) {
        parent->layout()->addWidget(widget);
    } else {
        LOG(WARNING) << "SEPropertyVector2::create without parent";
    }
    return widget;
}

std::reference_wrapper<const Vector2> SEPropertyVector2::getValue() const {
    return _Value;
}

void SEPropertyVector2::setDragFactor(float factor) {
    _DragVariableFactor.set(factor, factor);
    _XPosEdit->setDragFactor(factor);
    _YPosEdit->setDragFactor(factor);
}

void SEPropertyVector2::setDragFactor(Vector2 factor) {
    _DragVariableFactor = factor;
    _XPosEdit->setDragFactor(factor.x);
    _YPosEdit->setDragFactor(factor.y);
}

void SEPropertyVector2::setDragFactorX(float factor){
    _DragVariableFactor.x = factor;
    _XPosEdit->setDragFactor(factor);
}

void SEPropertyVector2::setDragFactorY(float factor){
    _DragVariableFactor.y = factor;
    _YPosEdit->setDragFactor(factor);
}

void SEPropertyVector2::valuesChanged() {
    _Value.x = _XPosEdit->text().toFloat();
    _Value.y = _YPosEdit->text().toFloat();

    refresh();
}

void SEPropertyVector2::refresh() {
    _XPosEdit->setText(QString::number(_Value.x));
    _YPosEdit->setText(QString::number(_Value.y));
}

void SEPropertyVector2::editingFinished() {
    refresh();
}

void SEPropertyVector2::mousePressEvent(QMouseEvent* event) {
    _IsDragging = true;
    _DragStartPosition = event->pos();
    QApplication::setOverrideCursor(Qt::SizeAllCursor);
}

void SEPropertyVector2::mouseReleaseEvent(QMouseEvent* event) {
    _IsDragging = false;
    QApplication::restoreOverrideCursor();
}

void SEPropertyVector2::mouseMoveEvent(QMouseEvent* event) {
    if (!_IsDragging) {
        return;
    }
    float changeX = (float) (_DragStartPosition.x() - event->pos().x()) *
                    _DragVariableFactor.x;
    float changeY = (float) (_DragStartPosition.y() - event->pos().y()) *
                    _DragVariableFactor.y;

    if ((changeX < 0.1f && changeX > -0.1f) &&
        (changeY < 0.1f && changeY > -0.1f)) {
        return;
    }

    float newX = _XPosEdit->text().toFloat() - changeX;
    float newY = _YPosEdit->text().toFloat() - changeY;

    _XPosEdit->setText(QString::number(newX));
    _YPosEdit->setText(QString::number(newY));

    valuesChanged();

    _DragStartPosition = event->pos();
}