#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "property_widgets/SEPropertyWidget.h"
#include "sol2/sol.hpp"

/* Widget used as root widget for all storm scene components*/

/*
 * TODO LUA PLAN:
 * - Bind lua script object to SERootComponentWidget widget
 * - Call "onCreated" lua method
 * - Create all property widgets from lua, and store reference to them in lua
 * - - Property widgets
 */

class SSceneComponent;

class SERootComponentWidget : public QWidget {
    Q_OBJECT

public:
    SERootComponentWidget(QWidget* parent);
    virtual ~SERootComponentWidget();

    /* Sets component to widget.
     * This method will load lua script and create lua handle
     * Returns < 0 on error */
    int loadComponent(SSceneComponent* component);

    /* Initialize widget.
     * Execute lua methods
     * Returns < 0 on error */
    int initialize();

public slots:
    /* Collapse / Uncollapse this widget */
    void toggleCollapse();

    void addPropertyWidget(SEPropertyWidget* widget);

protected:
    QVBoxLayout* _Layout;

    SSceneComponent* _Component;

    /* Lua table containing this widget's lua handle */
    sol::table _LuaHandle;

private:
    QPushButton* _ToggleButton;

};
