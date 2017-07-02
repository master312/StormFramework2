#include "stormglwidget.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QOpenGLFunctions>
#include <QSurfaceFormat>
#include "../../src/StormEngine.h"
#include "../../src/core/platforms/StormPlatformQt.h"

#include <iostream>
StormGlWidget::StormGlWidget(QWidget *parent) : QOpenGLWidget(parent) {
    _Timer = new QTimer(this);
    _Timer->setInterval(10);
    connect(_Timer, SIGNAL(timeout()), this, SLOT(timerTick()));

    QSurfaceFormat glFormat;
    glFormat.setVersion(2, 1);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    this->setFormat(glFormat);

    _StormPlatform = nullptr;
}

void StormGlWidget::initializeGL() {
    initializeOpenGLFunctions();
    QOpenGLWidget::initializeGL();

    _Timer->start();

    std::cout << "INITIALIZED GLL Asdasd asd  asdasd`1111111" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
   // uint32_t to;
   // glGenVertexArrays(1, &to);

    StormEngine::instance()->initialize(STORM_PLATFORM_QT);
    _StormPlatform = dynamic_cast<StormPlatformQt*>(StormEngine::instance()->getPlatform());
}

void StormGlWidget::resizeGL(int width, int height) {

}

void StormGlWidget::paintGL() {
    _StormPlatform->mainTick();
    /* Main thread here! */
   /* glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);                      // Drawing Using Triangles
        glVertex3f( 0.0f, 1.0f, 0.0f);              // Top
        glVertex3f(-1.0f,-1.0f, 0.0f);              // Bottom Left
        glVertex3f( 1.0f,-1.0f, 0.0f);              // Bottom Right
    glEnd();*/
    //std::cout << "asdasd " << std::endl;
}

void StormGlWidget::teardownGL() {

}

bool StormGlWidget::event(QEvent *e) {
    if (e->type() == QEvent::Move)
    {
      moveEvent(static_cast<QMoveEvent*>(e));
      return true;
    }
    else if (e->type() == QEvent::KeyPress)
    {
      keyPressEvent(static_cast<QKeyEvent*>(e));
      return true;
    }
    else if (e->type() == QEvent::KeyRelease)
    {
      keyReleaseEvent(static_cast<QKeyEvent*>(e));
      return true;
    }
    return QOpenGLWidget::event(e);
}

void StormGlWidget::keyPressEvent(QKeyEvent *event) {

}

void StormGlWidget::keyReleaseEvent(QKeyEvent *event) {

}

void StormGlWidget::mousePressEvent(QMouseEvent *event) {

}

void StormGlWidget::mouseReleaseEvent(QMouseEvent *event) {

}

void StormGlWidget::mouseMoveEvent(QMouseEvent *event) {
    std::cout << "mouseMoveEvent X " << event->pos().x() << " Y " << event->pos().y() << std::endl;
}

void StormGlWidget::moveEvent(QMoveEvent *event) {

}

void StormGlWidget::timerTick() {
    QOpenGLWidget::update();
}
