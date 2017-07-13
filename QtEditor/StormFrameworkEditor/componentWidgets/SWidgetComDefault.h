#pragma once
#include "../SWidgetComponent.h"

class SWidgetComDefault : public SWidgetComponent {
    Q_OBJECT

public:
    SWidgetComDefault(QWidget* parent = nullptr);

    virtual void initialize();

    virtual void refresh();
};
