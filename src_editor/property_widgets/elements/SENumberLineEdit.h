#pragma once
#include <QLineEdit>

/* Numeric text field widget whose value can be changed by dragging.
 * TODO: Int validator bug... */

enum SNumberLineEditValueType {
    S_NUMBER_LINE_EDIT_FLOAT,
    S_NUMBER_LINE_EDIT_INT
};

class SENumberLineEdit : public QLineEdit {
    Q_OBJECT

public:
    SENumberLineEdit(QWidget* parent = nullptr, SNumberLineEditValueType type = S_NUMBER_LINE_EDIT_FLOAT);
    virtual ~SENumberLineEdit();

    void setValueType(SNumberLineEditValueType type);
    void setDragFactor(float factor);

public slots:
    void editingFinished();

signals:
    void valueChanged();

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent* event);
    virtual void focusInEvent(QFocusEvent* event);
    virtual void focusOutEvent(QFocusEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);

private:
    static const int DRAG_THEESHOLD = 2;

    SNumberLineEditValueType _ValueType;
    float _ValueDragFactor;

    bool _IsPressed;
    bool _IsDragging;
    bool _IsFocused;
    QPoint _DragStartPosition;

    void handleDragging(QPoint pointerPos);
    void endDragging();
};
