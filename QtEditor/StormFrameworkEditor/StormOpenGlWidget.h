#ifndef STORMOPENGLWIDGET_H
#define STORMOPENGLWIDGET_H

/* Must be inculded before everything else */
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <QObject>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>

#include "../../src/StormEngine.h"
#include "../../src/core/platforms/StormPlatformQt.h"

class QEvent;
class QKeyEvent;
class QMouseEvent;

class StormOpenGlWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    StormOpenGlWidget(QWidget* parent = nullptr);
    virtual ~StormOpenGlWidget();

public slots:
    void mainTimerTick();

protected:
    /* Opengl protected methods */
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    virtual void paintGL();
    virtual void teardownGL();

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

#endif // STORMOPENGLWIDGET_H
