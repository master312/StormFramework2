#include "StormOpenGlWidget.h"
#include <QKeyEvent>
#include <QSurfaceFormat>

StormOpenGlWidget::StormOpenGlWidget(QWidget* parent /* = 0 */) : QOpenGLWidget(parent) {
    _Timer = new QTimer(this);
    _Timer->setInterval(10);
    connect(_Timer, SIGNAL(timeout()), this, SLOT(mainTimerTick()));

    QSurfaceFormat glFormat;
    /* Sets minimum opengl to 3.0 */
    glFormat.setVersion(2, 1);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    this->setFormat(glFormat);

    grabKeyboard();
}

StormOpenGlWidget::~StormOpenGlWidget() {
}

void StormOpenGlWidget::mainTimerTick() {
    update();
}

void StormOpenGlWidget::initializeGL() {
    initializeOpenGLFunctions();
    QOpenGLWidget::initializeGL();

    LOG(INFO) << "GL initialized " << glGetString(GL_VERSION);

    StormEngine::instance()->initialize(STORM_PLATFORM_QT);
    _StormPlatform = dynamic_cast<StormPlatformQt*>(StormEngine::instance()->getPlatform());

    _Timer->start();
}

void StormOpenGlWidget::resizeGL(int width, int height) {
}

void StormOpenGlWidget::paintGL() {
    _StormPlatform->mainTick();
}

void StormOpenGlWidget::teardownGL() {
    StormEngine::instance()->deinitialize();
}

bool StormOpenGlWidget::event(QEvent* event) {
    switch(event->type()) {
        /* TODO: Handle error event */
        case QEvent::KeyPress:
            keyPressEvent(static_cast<QKeyEvent*>(event));
            break;
        case QEvent::KeyRelease:
            keyReleaseEvent(static_cast<QKeyEvent*>(event));
            break;
    }
    return QOpenGLWidget::event(event);
}

void StormOpenGlWidget::keyPressEvent(QKeyEvent* event) {
    if (event->isAutoRepeat()) {
        return;
    }
    _StormPlatform->getInputManager()->processKeyEvent(true, (StormKey)(event->nativeVirtualKey() & 0x0000ff));
}

void StormOpenGlWidget::keyReleaseEvent(QKeyEvent* event) {
    if (event->isAutoRepeat()) {
        return;
    }
    _StormPlatform->getInputManager()->processKeyEvent(false, (StormKey)(event->nativeVirtualKey() & 0x0000ff));
}

void StormOpenGlWidget::mousePressEvent(QMouseEvent* event) {
    _StormPlatform->getInputManager()->processPointerButton((int)event->button(), true);
}

void StormOpenGlWidget::mouseReleaseEvent(QMouseEvent* event) {
    _StormPlatform->getInputManager()->processPointerButton((int)event->button(), false);
}

void StormOpenGlWidget::mouseMoveEvent(QMouseEvent* event) {
    _StormPlatform->getInputManager()->processPointerMotion(1, event->x(), event->y());
}
