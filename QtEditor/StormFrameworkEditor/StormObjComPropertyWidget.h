#pragma once
#include <QWidget>
#include <QLabel>


/* Base class for everty component property type. E.X: Color, Vector, Point, etc... */

class StormObjComponentWidget;

class StormObjComPropertyWidget : public QWidget {
    Q_OBJECT

public:
    StormObjComPropertyWidget(StormObjComponentWidget* parent = nullptr, const std::string& name = "");
    virtual ~StormObjComPropertyWidget();

    virtual void refresh();

protected:
    virtual void enterEvent(QEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void paintEvent(QPaintEvent* event);

    /* Name string of this vector variable that will be displayed in gui */
    std::string _Name;

    /* Pointer to parent component (for easy access) that contains all component property widgets */
    StormObjComponentWidget* _ComponentWidgetParent;

    /* Background opacity */
    float _BackgroundOpacity;

    /* Generated label widget with @_Name text set to it */
    QLabel* generateNameLabel();
};

