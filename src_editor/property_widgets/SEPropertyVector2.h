#pragma once

#include "SEPropertyWidget.h"
#include "elements/SENumberLineEdit.h"
#include "utils/math/Vector2.h"
#include <functional>

class SEPropertyVector2 : public SEPropertyWidget {
    Q_OBJECT

public:
    SEPropertyVector2(QWidget* parent = nullptr, const std::string& name = "");
    virtual ~SEPropertyVector2();

    void setValue(const Vector2& vec);
    void setValueX(const float x);
    void setValueY(const float y);

    std::reference_wrapper<const Vector2> getValue() const;

    void setDragFactor(float factor);
    void setDragFactor(Vector2 factor);
    void setDragFactorX(float factor);
    void setDragFactorY(float factor);

public slots:
    /* Signal when value in SNumberLineEdit fields changes */
    void valuesChanged();
    void editingFinished();

protected:
    /* Gets values from vector and put the in QText fields */
    virtual void refresh();
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);

private:
    Vector2 _Value;

    /* Pointer to text input widgets */
    SENumberLineEdit* _XPosEdit;
    SENumberLineEdit* _YPosEdit;

    bool _IsDragging;
    QPoint _DragStartPosition;
    Vector2 _DragVariableFactor;

    /* Reads values from getter and displays them in QLineEdit widgets */
    void readValues();
};
