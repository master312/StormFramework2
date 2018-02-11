#pragma once
#include <QWidget>
#include <QPushButton>

class SEComponentWidget : public QWidget{
    Q_OBJECT

public:
    SEComponentWidget(QWidget* parent);
    virtual ~SEComponentWidget();

private:
    QPushButton* _ToggleButton;

};
