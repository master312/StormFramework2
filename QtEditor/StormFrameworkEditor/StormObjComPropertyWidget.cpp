#include "StormObjComPropertyWidget.h"
#include "StormObjComponentWidget.h"

StormObjComPropertyWidget::StormObjComPropertyWidget(StormObjComponentWidget* parent, const std::string& name) : QWidget(parent) {
    _Name = name;
    _ComponentWidgetParent = parent;
}

StormObjComPropertyWidget::~StormObjComPropertyWidget() {

}

void StormObjComPropertyWidget::refresh() {

}

QLabel* StormObjComPropertyWidget::generateNameLabel() {
    QLabel* label = new QLabel(this);
    label->setText(std::string(_Name + ": ").c_str());

    return label;
}
