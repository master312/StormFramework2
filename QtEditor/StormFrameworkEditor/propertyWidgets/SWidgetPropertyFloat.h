#pragma once
#include <functional>
#include "../SNumberLineEdit.h"
#include "../StormObjComPropertyWidget.h"

class SWidgetPropertyFloat : public StormObjComPropertyWidget {
    Q_OBJECT

public:
    SWidgetPropertyFloat(StormObjComponentWidget* parent = nullptr, const std::string& name = "");

    virtual void refresh();

    void setValuePointer(float* value);
    void setSetter(std::function<void(float)> setter);
    void setGetter(std::function<float()> getter);

    void setDragFactor(float factor);

public slots:
    /* Signal when value in SNumberLineEdit value changes */
    void valueChanged();
    void editingFinished();

private:
    float* _FloatValue;
    std::function<void(float)> _FloatSetter;
    std::function<float()> _FloatGetter;

    SNumberLineEdit* _FloatTextEdit;
};
