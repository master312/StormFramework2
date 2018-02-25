#include "SENumberLineEdit.h"
#include <QApplication>
#include <QMouseEvent>
#include <QDoubleValidator>
#include <QIntValidator>

SENumberLineEdit::SENumberLineEdit(QWidget* parent, SNumberLineEditValueType type) : QLineEdit(parent) {
    _IsPressed = false;
    _IsDragging = false;
    _IsFocused = false;
    _DragStartPosition.setX(0);
    _DragStartPosition.setY(0);
    _ValueDragFactor = 1.0f;
    setValueType(type);

    connect(this, SIGNAL(editingFinished()), this, SLOT(editingFinished()));
}

SENumberLineEdit::~SENumberLineEdit() {
}

void SENumberLineEdit::setValueType(SNumberLineEditValueType type) {
    _ValueType = type;
    switch (_ValueType) {
        case S_NUMBER_LINE_EDIT_FLOAT:
            setValidator(new QDoubleValidator(this));
            break;
        case S_NUMBER_LINE_EDIT_INT:
            setValidator(new QIntValidator(this));
            break;
    }
}

void SENumberLineEdit::setDragFactor(float factor) {
    _ValueDragFactor = factor;
}

void SENumberLineEdit::editingFinished() {
    valueChanged();
    clearFocus();
}

void SENumberLineEdit::mouseDoubleClickEvent(QMouseEvent* event) {
    _IsFocused = true;
    setFocus();
    QLineEdit::mouseDoubleClickEvent(event);
}

void SENumberLineEdit::focusInEvent(QFocusEvent* event) {
    if (!_IsFocused) {
        clearFocus();
    }
}

void SENumberLineEdit::focusOutEvent(QFocusEvent* event) {
    _IsFocused = false;
    QLineEdit::focusOutEvent(event);
}

void SENumberLineEdit::mousePressEvent(QMouseEvent* event) {
    if (_IsFocused) {
        if (_IsPressed) {
            endDragging();
        }
        QLineEdit::mousePressEvent(event);
    } else {
        _IsPressed = true;
        _DragStartPosition = event->pos();
    }
    update();
}

void SENumberLineEdit::mouseReleaseEvent(QMouseEvent* event) {
    if (_IsFocused) {
        if (_IsPressed) {
            endDragging();
        }
        QLineEdit::mouseReleaseEvent(event);
    } else {
        valueChanged();
        endDragging();
        QApplication::restoreOverrideCursor();
    }
    update();
}

void SENumberLineEdit::mouseMoveEvent(QMouseEvent* event) {
    if (hasFocus()) {
        /* Widget is focued for editing */
        QLineEdit::mouseMoveEvent(event);
        update();
        return;
    }
    if (!_IsPressed) {
        return;
    }
    /* Widget value is currently dragging */
    handleDragging(event->pos());
}

void SENumberLineEdit::handleDragging(QPoint pointerPos) {
    int distance = _DragStartPosition.x() - pointerPos.x();
    if (!_IsDragging) {
        /* Actual dragging not started yet. Wait for treshold to be passed */
        if (abs(distance) >= DRAG_THEESHOLD) {
            _IsDragging = true;
            QApplication::setOverrideCursor(Qt::SizeHorCursor);
        }
        return;
    }
    float valueChangedAmount = (float)distance * _ValueDragFactor;

    if (_ValueType == S_NUMBER_LINE_EDIT_INT) {
        int number = text().toInt();
        number -= round(valueChangedAmount);
        setText(QString::number(number));
    } else if (_ValueType == S_NUMBER_LINE_EDIT_FLOAT) {
        float number = text().toFloat();
        number -= valueChangedAmount;
        setText(QString::number(number));
    }

    valueChanged();
    _DragStartPosition = pointerPos;
}

void SENumberLineEdit::endDragging() {
    _IsPressed = false;
    _IsDragging = false;
    _DragStartPosition.setX(0);
    _DragStartPosition.setY(0);
    QLineEdit::editingFinished();
}
