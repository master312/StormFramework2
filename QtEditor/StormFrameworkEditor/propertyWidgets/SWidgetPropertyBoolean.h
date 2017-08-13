#pragma once
#include <functional>
#include "../SWidgetProperty.h"

class QCheckBox;

class SWidgetPropertyBoolean : public SWidgetProperty {
    Q_OBJECT

public:
    SWidgetPropertyBoolean(SWidgetComponent* parent = nullptr, const std::string& name = "");

    virtual void refresh();

    void setValuePointer(bool* value);
    void setGetter(std::function<bool()> getter);
    void setSetter(std::function<void(bool)> setter);

public slots:
    /* Signal when bool value gets changed */
    void valueChanged(int value);

private:
    bool* _BoolValue;

    std::function<void(bool)> _BoolSetter;
    std::function<bool()> _BoolGetter;


    QCheckBox* _Checkbox;
};

