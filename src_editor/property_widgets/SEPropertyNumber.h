#pragma once

#include "SEPropertyWidget.h"
#include "elements/SENumberLineEdit.h"
#include "sol2/sol.hpp"

enum SEPropertyNumberType {
    SE_PROPERTY_NUM_FLOAT,
    SE_PROPERTY_NUM_INT,
    SE_PROPERTY_NUM_ANGLE
};

class SEPropertyNumber : public SEPropertyWidget {
    Q_OBJECT

public:
    SEPropertyNumber(QWidget* parent = nullptr, const std::string& name = "");
    virtual ~SEPropertyNumber();

    /* Factory method for producing SEPropertyNumber objects
     * @type string can be "float", "int", "angle" */
    static SEPropertyNumber* create(QWidget* parent, const std::string& name, const std::string& type);

    /* Float / Int / Angle */
    void setValueType(SEPropertyNumberType type);

    void setValue(float number);
    float getValue();

public slots:
    /* Signal when value in SNumberLineEdit fields changes */
    void valuesChanged();
    void editingFinished();

protected:
    virtual void refresh();

private:
    float _Value;
    SENumberLineEdit* _ValueEdit;
    SEPropertyNumberType _ValueType;

    /* Modify @_Value based on type. (EX: For angle value will clamped between -360 and 360) */
    void processValueType();
};
