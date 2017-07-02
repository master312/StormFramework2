#pragma once
#include <QLineEdit>
#include <functional>
#include "../../src/core/utils/math/Vector2.h"
#include "../StormObjComPropertyWidget.h"
class StormObjComponentWidget;

class StormDragLineEdit : public QLineEdit {
public:
    StormDragLineEdit(QWidget* parent = nullptr);

protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);

private:
    bool _IsDragging;
    QPoint _DragStartPosition;
};

class StormPropertyVec2Widget : public StormObjComPropertyWidget {
    Q_OBJECT

public:
    StormPropertyVec2Widget(StormObjComponentWidget* parent = nullptr, const std::string& name = "");
    virtual ~StormPropertyVec2Widget();

    virtual void refresh();

    void setVector(Vector2* vector);

    void setVectorSetter(std::function<void(float, float)> setter);
    void setVectorSetter(std::function<void(Vector2)> setter);
    void setVectorGetter(std::function<Vector2()> getter);

public slots:
    /* Signal received when finished editing QLineEdit text fields */
    void editingFinished();

private:
    /* Vector setter and getted methods */
    std::function<void(Vector2)> _VectorSetter;
    std::function<void(float, float)> _VectorFloatSetter;
    std::function<Vector2()> _VectorGetter;

    /* Pointer to text input widgets */
    QLineEdit* _XPosEdit;
    QLineEdit* _YPosEdit;

    /* Reads values from getter and displays them in QLineEdit widgets */
    void readValues();
};
