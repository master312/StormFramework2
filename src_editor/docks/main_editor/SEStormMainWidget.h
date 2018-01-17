#pragma once

/* Must be included before everything else */
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

/* Hack fix for compiler warnings */
#undef __GLEW_H__
#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#define __GLEW_H__

#include <QTimer>

#include "StormEngine.h"
#include "platforms/StormPlatformQt.h"

class QEvent;
class QKeyEvent;
class QMouseEvent;
class StormEngine;

class SEStormMainWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    SEStormMainWidget(QWidget* parent = nullptr);
    virtual ~SEStormMainWidget();

public slots:
    void mainTimerTick();

protected:
    /* Opengl protected methods */
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    virtual void paintGL();

    /* Event listener methods */
    virtual bool event(QEvent* event);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);

private:
    /* Main ticking timer */
    QTimer* _Timer;

    StormPlatformQt* _StormPlatform;
};
