#include "SWidgetPropertyBoolean.h"
#include <QCheckBox>
#include <QHBoxLayout>

SWidgetPropertyBoolean::SWidgetPropertyBoolean(SWidgetComponent* parent, const std::string& name) : SWidgetProperty(parent, name) {
    _BoolValue = nullptr;
    _BoolGetter = nullptr;
    _BoolSetter = nullptr;

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(1);

    _Checkbox = new QCheckBox(this);

    layout->addWidget(generateNameLabel());
    layout->addWidget(_Checkbox);

    connect(_Checkbox, SIGNAL(stateChanged(int)), this, SLOT(valueChanged(int)));

    setLayout(layout);
}

void SWidgetPropertyBoolean::refresh() {
    if (_BoolGetter) {
        _Checkbox->setChecked(_BoolGetter());
    } else if (_BoolValue) {
        _Checkbox->setChecked(*_BoolValue);
    }
}

void SWidgetPropertyBoolean::setValuePointer(bool* value) {
    _BoolValue = value;
}

void SWidgetPropertyBoolean::setGetter(std::function<bool()> getter) {
    _BoolGetter = getter;
}

void SWidgetPropertyBoolean::setSetter(std::function<void(bool)> setter) {
    _BoolSetter = setter;
}

void SWidgetPropertyBoolean::valueChanged(int value) {
    if (_BoolValue) {
        *_BoolValue = value > 0;
        return;
    }
    if (_BoolSetter) {
        _BoolSetter(value > 0);
    }
}
