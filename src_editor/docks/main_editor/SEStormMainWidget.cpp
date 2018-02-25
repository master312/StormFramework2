#include "SEStormMainWidget.h"
#include <QApplication>
#include <QKeyEvent>
#include <src/StormDebug.h>
#include "core/resources/StormFileSystem.h"
#include "MainWindow.h"
#include "../object_hierarchy/SEDockObjectHierarchy.h"

SEStormMainWidget::SEStormMainWidget(QWidget* parent) : QOpenGLWidget(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    _Timer = new QTimer(this);
    _Timer->setInterval(10);
    connect(_Timer, SIGNAL(timeout()), this, SLOT(mainTimerTick()));

    QSurfaceFormat glFormat;
    /* Sets minimum opengl to 3.0 */
    glFormat.setVersion(3, 0);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
    this->setFormat(glFormat);

    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

SEStormMainWidget::~SEStormMainWidget() {
    /* TODO: Better deinitialization. Don't destroy in destructor like this */
    StormEngine::instance()->deinitialize();
}

void SEStormMainWidget::mainTimerTick() {
    update();
}

void SEStormMainWidget::initializeGL() {
    initializeOpenGLFunctions();
    QOpenGLWidget::initializeGL();

    LOG(INFO) << "GL initialized " << glGetString(GL_VERSION);

    StormEngine::instance()->initialize(STORM_PLATFORM_QT);
    _StormPlatform = dynamic_cast<StormPlatformQt*>(StormEngine::getModule<StormPlatform>());

    _Timer->start();

    engineInitialized();
}

void SEStormMainWidget::resizeGL(int width, int height) {
    _StormPlatform->handleWidgetResize(width, height);
}

void SEStormMainWidget::paintGL() {
    _StormPlatform->mainTick();
}

bool SEStormMainWidget::event(QEvent* event) {
    switch(event->type()) {
        /* TODO: Handle error event */
        case QEvent::KeyPress:
            keyPressEvent(static_cast<QKeyEvent*>(event));
            break;
        case QEvent::KeyRelease:
            keyReleaseEvent(static_cast<QKeyEvent*>(event));
            break;
        case QEvent::WindowActivate: {
            LOG(DEBUG) << "Window focus restored. Reloading resources";
            StormFileSystem* fileSystem = StormEngine::getModule<StormFileSystem>();
            fileSystem->reloadResources();
            } break;
        default:
            break;
    }
    return QOpenGLWidget::event(event);
}

void SEStormMainWidget::keyPressEvent(QKeyEvent* event) {
    if (event->isAutoRepeat()) {
        return;
    }
    _StormPlatform->getInputManager()->processKeyEvent(true, (StormKey)(event->nativeVirtualKey() & 0x0000ff));
}

void SEStormMainWidget::keyReleaseEvent(QKeyEvent* event) {
    if (event->isAutoRepeat()) {
        return;
    }
    _StormPlatform->getInputManager()->processKeyEvent(false, (StormKey)(event->nativeVirtualKey() & 0x0000ff));
}

void SEStormMainWidget::mousePressEvent(QMouseEvent* event) {
    _StormPlatform->getInputManager()->processPointerButton((int)event->button(), true);
}

void SEStormMainWidget::mouseReleaseEvent(QMouseEvent* event) {
    _StormPlatform->getInputManager()->processPointerButton((int)event->button(), false);
}

void SEStormMainWidget::mouseMoveEvent(QMouseEvent* event) {
    _StormPlatform->getInputManager()->processPointerMotion(1, event->x(), event->y());
}
