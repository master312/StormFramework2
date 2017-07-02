#ifndef STORMGLWIDGET_H
#define STORMGLWIDGET_H
#include <GL/glew.h>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>

class QEvent;
class QGestureEvent;
class QKeyEvent;
class QMouseEvent;
class QMoveEvent;
class StormPlatformQt;

class StormGlWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    StormGlWidget(QWidget *parent = 0);

public slots:
    void timerTick();

protected:
    /* Opengl protected methods */
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    virtual void paintGL();
    virtual void teardownGL();

    /* Event listener methods */
    virtual bool event(QEvent *e);
    //virtual void errorEventGL(OpenGLError *event);
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void keyReleaseEvent(QKeyEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void moveEvent(QMoveEvent *event);

private:
    /* Main timer. Used for determening uptime and ticking */
    QTimer* _Timer;

    StormPlatformQt* _StormPlatform;
};

#endif // STORMGLWIDGET_H
