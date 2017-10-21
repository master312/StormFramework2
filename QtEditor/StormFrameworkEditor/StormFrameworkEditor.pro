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
DEFINES += QT_DEPRECATED_WARNINGS STORM_BUILD_PLATFORM_QT _DEBUG

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14

QMAKE_CXXFLAGS_RELEASE -= -O

INCLUDEPATH += /usr/include/lua5.1/
INCLUDEPATH += /usr/lib/
INCLUDEPATH += ../../libs/sol2/

LIBS += -lSDL2 -lSDL2_image -lGLEW -lGL -lGLU -llua5.1

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
    ../../libs/pugixml/pugixml.cpp \
    ../../src/core/platforms/StormPlatformQt.cpp \
    StormOpenGlWidget.cpp \
    SceneObjectsTreeWidget.cpp \
    SNumberLineEdit.cpp \
    propertyWidgets/SWidgetPropertyFloat.cpp \
    propertyWidgets/SWidgetPropertyVec2.cpp \
    propertyWidgets/SWidgetPropertyResource.cpp \
    SWidgetProperty.cpp \
    componentWidgets/SWidgetComPlane.cpp \
    componentWidgets/SWidgetComStaticTexture.cpp \
    SWidgetComponent.cpp \
    SResourceLineEdit.cpp \
    SceneObjectTreeWidgetItem.cpp \
    ../../src/scene/components/SSceneSystemPlane.cpp \
    propertyWidgets/SWidgetPropertyColor.cpp \
    componentWidgets/SWidgetComTransform.cpp \
    ../../src/scene/components/SSceneSystemTransform.cpp \
    ../../src/scene/components/SSceneSystemLuaScript.cpp \
    ../../src/scene/components/SSceneComTransform.cpp \
    ../../src/scene/components/SSceneComLuaScript.cpp \
    propertyWidgets/SWidgetPropertyBoolean.cpp \
    ../../src/scene/SSceneManager.cpp \
    ../../src/SEngineModuleFactory.cpp \
    ../../libs/imgui/imgui.cpp \
    ../../libs/imgui/imgui_demo.cpp \
    ../../libs/imgui/imgui_draw.cpp \
    ../../src/scene/components/lua/SLuaBinders.cpp \
    ../../libs/easyloggingpp/easylogging++.cpp

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
    ../../libs/pugixml/pugiconfig.hpp \
    ../../libs/pugixml/pugixml.hpp \
    ../../src/core/platforms/StormPlatformQt.h \
    StormOpenGlWidget.h \
    SceneObjectsTreeWidget.h \
    StormQtHelperFunctions.h \
    SNumberLineEdit.h \
    propertyWidgets/SWidgetPropertyFloat.h \
    propertyWidgets/SWidgetPropertyVec2.h \
    propertyWidgets/SWidgetPropertyResource.h \
    SWidgetProperty.h \
    componentWidgets/SWidgetComPlane.h \
    componentWidgets/SWidgetComStaticTexture.h \
    SWidgetComponent.h \
    SResourceLineEdit.h \
    SceneObjectTreeWidgetItem.h \
    ../../src/scene/components/SSceneSystemPlane.h \
    propertyWidgets/SWidgetPropertyColor.h \
    componentWidgets/SWidgetComTransform.h \
    ../../src/scene/components/SSceneSystemTransform.h \
    ../../src/scene/components/SSceneSystemLuaScript.h \
    ../../src/scene/components/SSceneComTransform.h \
    propertyWidgets/SWidgetPropertyBoolean.h \
    ../../src/scene/SSceneManager.h \
    ../../src/SEngineModuleFactory.h \
    ../../libs/imgui/imconfig.h \
    ../../libs/imgui/imgui.h \
    ../../libs/imgui/imgui_internal.h \
    ../../libs/imgui/stb_rect_pack.h \
    ../../libs/imgui/stb_textedit.h \
    ../../libs/imgui/stb_truetype.h \
    ../../src/scene/components/lua/SLuaBinders.h \
    ../../src/scene/SSceneObjectEventTypes.h \
    ../../libs/easyloggingpp/easylogging++.h

FORMS += \
        mainwindow.ui

DISTFILES +=
