#pragma once
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include "sol2/sol.hpp"

/* Base class for all property widgets
 * Default layout (@_Layout) is vBox layout
 */

class SEPropertyWidget : public QWidget {
public:
    SEPropertyWidget(QWidget* parent, const std::string& name = "Property widget");
    virtual ~SEPropertyWidget();

    /* Sets lua method that will be called every time value
     * has been changed from QT interface */
    virtual void setValueChangedLuaListener(sol::function callback);

public slots:
    /* Signal fiered to refresh property values */
    void refreshValues();

protected:
    std::string _Name;
    QLayout* _Layout;

    /* Background opacity used for hover effect */
    float _BackgroundOpacity;

    /* Lua function to be executed on value change */
    sol::function _ValueChangedCallback;

    /* Refresh property values */
    virtual void refresh();

    virtual void enterEvent(QEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void paintEvent(QPaintEvent* event);

    /* Generates title label with text @text */
    QLabel* generateLabel(const QString& text);

    static QLayout* generateVBoxLayout();
};
