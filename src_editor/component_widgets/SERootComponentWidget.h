#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "property_widgets/SEPropertyWidget.h"

/* Widget used as root widget for all storm scene components*/

/*
 * TODO LUA PLAN:
 * - Bind lua script object to SERootComponentWidget widget
 * - Call "onCreated" lua method
 * - Create all property widgets from lua, and store reference to them in lua
 * - - Property widgets
 */

class SERootComponentWidget : public QWidget {
    Q_OBJECT

public:
    SERootComponentWidget(QWidget* parent);
    virtual ~SERootComponentWidget();

public slots:
    /* Collapse / Uncollapse this widget */
    void toggleCollapse();

    void addPropertyWidget(SEPropertyWidget* widget);

protected:
    QVBoxLayout* _Layout;

private:
    QPushButton* _ToggleButton;

};
