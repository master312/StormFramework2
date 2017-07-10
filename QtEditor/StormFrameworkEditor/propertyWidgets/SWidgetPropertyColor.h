#pragma once
#include <QLineEdit>
#include "../SWidgetProperty.h"
#include "../../src/core/utils/math/Color.h"

class SWidgetPropertyColor : public SWidgetProperty {
    Q_OBJECT

public:
    SWidgetPropertyColor(SWidgetComponent* parent = nullptr, const std::string& name = "");

    virtual void refresh();

    void setValuePointer(Color* color);

protected:
    virtual void mousePressEvent(QMouseEvent* event);

private:
    Color* _Color;

    QLineEdit* _ColorPriview;
};
