#-------------------------------------------------
#
# Project created by QtCreator 2017-06-28T21:54:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StormFrameworkEditor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS STORM_BUILD_PLATFORM_QT

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

QMAKE_CXXFLAGS_RELEASE -= -O


LIBS += -lSDL2 -lSDL2_image -lGLEW -lGL -lGLU

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    ../../src/core/graphics/StormRenderer.cpp \
    ../../src/core/graphics/StormShader.cpp \
    ../../src/core/graphics/StormTexture.cpp \
    ../../src/core/graphics/StormVideoDriver.cpp \
    ../../src/core/platforms/StormInputManager.cpp \
    ../../src/core/platforms/StormPlatform.cpp \
    ../../src/core/platforms/StormPlatformSDL2.cpp \
    ../../src/core/resources/StormFileSystem.cpp \
    ../../src/core/resources/StormResourceFile.cpp \
    ../../src/core/utils/Plane.cpp \
    ../../src/core/utils/StormMiscTools.cpp \
    ../../src/scene/components/SSceneComPlane.cpp \
    ../../src/scene/components/SSceneComStaticTexture.cpp \
    ../../src/scene/components/SSceneSystemStaticTexture.cpp \
    ../../src/scene/SSceneComponent.cpp \
    ../../src/scene/SSceneComponentSystem.cpp \
    ../../src/scene/StormScene.cpp \
    ../../src/scene/StormSceneObject.cpp \
    ../../src/StormEngine.cpp \
    ../../src/StormTextureManager.cpp \
    ../../libs/easyloggingpp/easylogging++.cpp \
    ../../libs/pugixml/pugixml.cpp \
    ../../src/core/platforms/StormPlatformQt.cpp \
    StormOpenGlWidget.cpp \
    SceneObjectsTreeWidget.cpp

HEADERS += \
        mainwindow.h \
    ../../src/core/graphics/StormRenderer.h \
    ../../src/core/graphics/StormShader.h \
    ../../src/core/graphics/StormTexture.h \
    ../../src/core/graphics/StormVideoDriver.h \
    ../../src/core/platforms/StormInputManager.h \
    ../../src/core/platforms/StormKeys.h \
    ../../src/core/platforms/StormPlatform.h \
    ../../src/core/platforms/StormPlatformSDL2.h \
    ../../src/core/resources/StormFileSystem.h \
    ../../src/core/resources/StormResourceFile.h \
    ../../src/core/utils/math/Color.h \
    ../../src/core/utils/math/Matrix.h \
    ../../src/core/utils/math/Rect.h \
    ../../src/core/utils/math/ScalarMath.h \
    ../../src/core/utils/math/TrigonometryMath.h \
    ../../src/core/utils/math/Vector2.h \
    ../../src/core/utils/math/Vector3.h \
    ../../src/core/utils/math/Vector4.h \
    ../../src/core/utils/Plane.h \
    ../../src/core/utils/StormIntrusivePtr.h \
    ../../src/core/utils/StormMiscTools.h \
    ../../src/core/utils/StormObjectPool.h \
    ../../src/core/utils/StormRefCounter.h \
    ../../src/core/utils/StormVertex.h \
    ../../src/core/StormCommon.h \
    ../../src/scene/components/SSceneComPlane.h \
    ../../src/scene/components/SSceneComStaticTexture.h \
    ../../src/scene/components/SSceneSystemStaticTexture.h \
    ../../src/scene/SSceneComponent.h \
    ../../src/scene/SSceneComponentSystem.h \
    ../../src/scene/StormScene.h \
    ../../src/scene/StormSceneObject.h \
    ../../src/StormEngine.h \
    ../../src/StormTextureManager.h \
    ../../libs/easyloggingpp/easylogging++.h \
    ../../libs/pugixml/pugiconfig.hpp \
    ../../libs/pugixml/pugixml.hpp \
    ../../src/core/platforms/StormPlatformQt.h \
    StormOpenGlWidget.h \
    SceneObjectsTreeWidget.h \
    StormQtHelperFunctions.h

FORMS += \
        mainwindow.ui
