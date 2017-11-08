#pragma once
#include <QWidget>
#include <QLabel>
#include <QTimer>
#include "SWidgetComponent.h"

/* Base class for everty component property type. E.X: Color, Vector, Point, etc... */

class SWidgetProperty : public QWidget {
    Q_OBJECT

public:
    SWidgetProperty(SWidgetComponent* parent = nullptr, const std::string& name = "");
    virtual ~SWidgetProperty();

    virtual void refresh();

public slots:
    /* Signal fiered from timer to update values */
    virtual void refreshValues();

protected:
    virtual void enterEvent(QEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void paintEvent(QPaintEvent* event);

    /* Timer that ticks at interval to refresh values */
    QTimer* _RefreshTimer;

    /* Name string of this vector variable that will be displayed in gui */
    std::string _Name;

    /* Pointer to parent component (for easy access) that contains all component property widgets */
    SWidgetComponent* _ComponentWidgetParent;

    /* Background opacity */
    float _BackgroundOpacity;

    /* Generated label widget with @_Name text set to it */
    QLabel* generateNameLabel();
};

