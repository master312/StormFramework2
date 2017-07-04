#include "SWidgetPropertyFloat.h"
#include <QHBoxLayout>

SWidgetPropertyFloat::SWidgetPropertyFloat(SWidgetComponent* parent, const std::string& name) : SWidgetProperty(parent, name) {
    _FloatValue = nullptr;
    _FloatGetter = nullptr;
    _FloatSetter = nullptr;

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setMargin(0);
    layout->setSpacing(1);

    _FloatTextEdit = new SNumberLineEdit(this);
    _FloatTextEdit->setDragFactor(0.5f);

    layout->addWidget(generateNameLabel());
    layout->addWidget(_FloatTextEdit);

    setLayout(layout);

    connect(_FloatTextEdit, SIGNAL(valueChanged()), this, SLOT(valueChanged()));
    connect(_FloatTextEdit, SIGNAL(editingFinished()), this, SLOT(editingFinished()));
}

void SWidgetPropertyFloat::refresh() {
    SWidgetProperty::refresh();

    if (_FloatValue) {
        _FloatTextEdit->setText(QString::number(*_FloatValue));
    } else if (_FloatGetter) {
        float number = _FloatGetter();
        _FloatTextEdit->setText(QString::number(number));
    } else {
        /* TODO: Log error */
    }
}

void SWidgetPropertyFloat::setValuePointer(float* value) {
    _FloatValue = value;
}

void SWidgetPropertyFloat::setSetter(std::function<void(float)> setter){
    _FloatSetter = setter;
}

void SWidgetPropertyFloat::setGetter(std::function<float()> getter){
    _FloatGetter = getter;
}

void SWidgetPropertyFloat::setDragFactor(float factor) {
    _FloatTextEdit->setDragFactor(factor);
}

void SWidgetPropertyFloat::valueChanged() {
    if (_FloatValue) {
        *_FloatValue = _FloatTextEdit->text().toFloat();
    } else if (_FloatSetter) {
        _FloatSetter(_FloatTextEdit->text().toFloat());
    } else {
        /* TODO: Log error */
    }

    _ComponentWidgetParent->refresh();
}

void SWidgetPropertyFloat::editingFinished() {
    refresh();
}
