#include "stormglwidget.h"
#include <QKeyEvent>
#include <QCoreApplication>
#include <QOpenGLFunctions>

#include <iostream>
StormGlWidget::StormGlWidget(QWidget *parent) : QOpenGLWidget(parent) {
    std::cout << "CREATEDDDD!!" << std::endl;
    _Timer = new QTimer(this);
    _Timer->setInterval(10);
    connect(_Timer, SIGNAL(timeout()), this, SLOT(timerTick()));
}

void StormGlWidget::initializeGL() {
    initializeOpenGLFunctions();
    QOpenGLWidget::initializeGL();

    _Timer->start();

    std::cout << "INITIALIZED GLL Asdasd asd  asdasd`1111111" << std::endl;
    //grabMouse();
}

void StormGlWidget::resizeGL(int width, int height) {

}

void StormGlWidget::paintGL() {
    /* Main thread here! */
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBegin(GL_TRIANGLES);                      // Drawing Using Triangles
        glVertex3f( 0.0f, 1.0f, 0.0f);              // Top
        glVertex3f(-1.0f,-1.0f, 0.0f);              // Bottom Left
        glVertex3f( 1.0f,-1.0f, 0.0f);              // Bottom Right
    glEnd();
    std::cout << "asdasd " << std::endl;
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
