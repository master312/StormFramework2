#include "SEPropertyNumber.h"
#include "StormCommon.h"
#include "utils/math/ScalarMath.h"

SEPropertyNumber::SEPropertyNumber(QWidget* parent /* = nullptr */, const std::string& name /* = "" */) : SEPropertyWidget(parent, name) {
    _Value = 0;
    _ValueEdit = new SENumberLineEdit(this);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(1);

    layout->addWidget(generateLabel(_Name.c_str()));
    layout->addWidget(_ValueEdit);

    connect(_ValueEdit, SIGNAL(valueChanged()), this, SLOT(valuesChanged()));
    connect(_ValueEdit, SIGNAL(editingFinished()), this, SLOT(editingFinished()));

    setLayout(layout);
    refresh();
}

SEPropertyNumber::~SEPropertyNumber() {
}

SEPropertyNumber* SEPropertyNumber::create(QWidget* parent, const std::string& name, const std::string& type) {
    SEPropertyNumberType selectedType = SE_PROPERTY_NUM_FLOAT;
    if (type == "int") {
        selectedType = SE_PROPERTY_NUM_INT;
    } else if (type == "angle") {
        selectedType = SE_PROPERTY_NUM_ANGLE;
    }
    SEPropertyNumber* widget = new SEPropertyNumber(parent, name);
    if (parent) {
        parent->layout()->addWidget(widget);
    } else {
        LOG(WARNING) << "SEPropertyNumber::create without parent";
    }
    widget->setValueType(selectedType);
    return widget;
}

void SEPropertyNumber::setValueType(SEPropertyNumberType type) {
    _ValueType = type;
    switch(_ValueType) {
        case SE_PROPERTY_NUM_FLOAT:
            _ValueEdit->setValueType(S_NUMBER_LINE_EDIT_FLOAT);
            break;
        case SE_PROPERTY_NUM_INT:
            _ValueEdit->setValueType(S_NUMBER_LINE_EDIT_INT);
            break;
        case SE_PROPERTY_NUM_ANGLE:
            _ValueEdit->setValueType(S_NUMBER_LINE_EDIT_FLOAT);
            break;
        default:
            LOG(ERROR) << "Invalid SEPropertyNumber value type";
            break;
    }
}

void SEPropertyNumber::setValue(float number) {
    _Value = number;
    processValueType();
    refresh();
}

float SEPropertyNumber::getValue() {
    return _Value;
}

void SEPropertyNumber::refresh() {
    if (!_ValueEdit->hasFocus()) {
        _ValueEdit->setText(QString::number(_Value));
    }
}

void SEPropertyNumber::editingFinished() {
    refresh();
}

void SEPropertyNumber::valuesChanged() {
    _Value = _ValueEdit->text().toFloat();
    processValueType();

    if (_ValueChangedCallback.valid()) {
        /* Execute LUA value changed callback */
        _ValueChangedCallback(_Value);
    }
}

void SEPropertyNumber::processValueType() {
    float oldValue = _Value;
    switch(_ValueType) {
        case SE_PROPERTY_NUM_INT:
            _Value = roundf(_Value);
            break;
        case SE_PROPERTY_NUM_ANGLE:
            _Value = StormScalarMath::clampAngle(_Value);
            break;
        default:
            break;
    }

    if (!StormScalarMath::equivalent(_Value, oldValue)) {
        /* Values are not equal. */
        refresh();
    }
}