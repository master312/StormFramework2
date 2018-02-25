#pragma once
#include "../SEDockWidget.h"

class QLayout;

class SEDockDebugSettings : public SEDockWidget {
    Q_OBJECT

public:
    SEDockDebugSettings(QMainWindow* parent);
    virtual ~SEDockDebugSettings();

public slots:
    /* Called every time system tick enabled checkbox changes value */
    void systemTickChanged(int value);

private:
    void generateSystemTickingCheckBoxes(QLayout* layout);
};
