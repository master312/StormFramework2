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
    std::string _Name;
    StormObjComponentWidget* _ComponentWidgetParent;

    QLabel* generateNameLabel();
};

