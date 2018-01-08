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
INCLUDEPATH += ../../libs/
INCLUDEPATH += ../../src/
INCLUDEPATH += ../../src/core

LIBS += -lSDL2 -lSDL2_image -lGLEW -lGL -lGLU -llua5.1

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    StormOpenGlWidget.cpp \
    SceneObjectsTreeWidget.cpp \
    SNumberLineEdit.cpp \
    propertyWidgets/SWidgetPropertyFloat.cpp \
    propertyWidgets/SWidgetPropertyVec2.cpp \
    propertyWidgets/SWidgetPropertyResource.cpp \
    SWidgetProperty.cpp \
    SWidgetComponent.cpp \
    SResourceLineEdit.cpp \
    SceneObjectTreeWidgetItem.cpp \
    propertyWidgets/SWidgetPropertyColor.cpp \
    componentWidgets/SWidgetComTransform.cpp \
    propertyWidgets/SWidgetPropertyBoolean.cpp \
    componentWidgets/SWidgetComScript.cpp \
    selectprefab.cpp \
    ../../src/core/graphics/StormRenderer.cpp \
    ../../src/core/graphics/StormShader.cpp \
    ../../src/core/graphics/StormTexture.cpp \
    ../../src/core/graphics/StormVideoDriver.cpp \
    ../../src/core/platforms/StormInputManager.cpp \
    ../../src/core/platforms/StormPlatform.cpp \
    ../../src/core/platforms/StormPlatformQt.cpp \
    ../../src/core/platforms/StormPlatformSDL2.cpp \
    ../../src/core/resources/StormFileSystem.cpp \
    ../../src/core/resources/StormResourceFile.cpp \
    ../../src/core/utils/Geometry.cpp \
    ../../src/core/utils/Plane.cpp \
    ../../src/core/utils/StormMiscTools.cpp \
    ../../src/scene/components/luaScript/SLuaBinders.cpp \
    ../../src/scene/components/luaScript/SSceneComLuaScript.cpp \
    ../../src/scene/components/luaScript/SSceneSystemLuaScript.cpp \
    ../../src/scene/components/physics/SSceneComPhysics.cpp \
    ../../src/scene/components/physics/SSceneSystemPhysics.cpp \
    ../../src/scene/components/sprite/SSceneComSprite.cpp \
    ../../src/scene/components/sprite/SSceneSystemSprite.cpp \
    ../../src/scene/components/transform/SSceneComTransform.cpp \
    ../../src/scene/components/transform/SSceneSystemTransform.cpp \
    ../../src/scene/SSceneComponent.cpp \
    ../../src/scene/SSceneComponentSystem.cpp \
    ../../src/scene/SSceneManager.cpp \
    ../../src/scene/SScene.cpp \
    ../../src/scene/SSceneObject.cpp \
    ../../src/SEngineModuleFactory.cpp \
    ../../src/StormDebug.cpp \
    ../../src/StormEngine.cpp \
    ../../src/StormTextureManager.cpp \
    ../../libs/Box2D/Collision/Shapes/b2ChainShape.cpp \
    ../../libs/Box2D/Collision/Shapes/b2CircleShape.cpp \
    ../../libs/Box2D/Collision/Shapes/b2EdgeShape.cpp \
    ../../libs/Box2D/Collision/Shapes/b2PolygonShape.cpp \
    ../../libs/Box2D/Collision/b2BroadPhase.cpp \
    ../../libs/Box2D/Collision/b2CollideCircle.cpp \
    ../../libs/Box2D/Collision/b2CollideEdge.cpp \
    ../../libs/Box2D/Collision/b2CollidePolygon.cpp \
    ../../libs/Box2D/Collision/b2Collision.cpp \
    ../../libs/Box2D/Collision/b2Distance.cpp \
    ../../libs/Box2D/Collision/b2DynamicTree.cpp \
    ../../libs/Box2D/Collision/b2TimeOfImpact.cpp \
    ../../libs/Box2D/Common/b2BlockAllocator.cpp \
    ../../libs/Box2D/Common/b2Draw.cpp \
    ../../libs/Box2D/Common/b2Math.cpp \
    ../../libs/Box2D/Common/b2Settings.cpp \
    ../../libs/Box2D/Common/b2StackAllocator.cpp \
    ../../libs/Box2D/Common/b2Timer.cpp \
    ../../libs/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
    ../../libs/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
    ../../libs/Box2D/Dynamics/Contacts/b2CircleContact.cpp \
    ../../libs/Box2D/Dynamics/Contacts/b2Contact.cpp \
    ../../libs/Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
    ../../libs/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
    ../../libs/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
    ../../libs/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
    ../../libs/Box2D/Dynamics/Contacts/b2PolygonContact.cpp \
    ../../libs/Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
    ../../libs/Box2D/Dynamics/Joints/b2FrictionJoint.cpp \
    ../../libs/Box2D/Dynamics/Joints/b2GearJoint.cpp \
    ../../libs/Box2D/Dynamics/Joints/b2Joint.cpp \
    ../../libs/Box2D/Dynamics/Joints/b2MotorJoint.cpp \
    ../../libs/Box2D/Dynamics/Joints/b2MouseJoint.cpp \
    ../../libs/Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
    ../../libs/Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
    ../../libs/Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
    ../../libs/Box2D/Dynamics/Joints/b2RopeJoint.cpp \
    ../../libs/Box2D/Dynamics/Joints/b2WeldJoint.cpp \
    ../../libs/Box2D/Dynamics/Joints/b2WheelJoint.cpp \
    ../../libs/Box2D/Dynamics/b2Body.cpp \
    ../../libs/Box2D/Dynamics/b2ContactManager.cpp \
    ../../libs/Box2D/Dynamics/b2Fixture.cpp \
    ../../libs/Box2D/Dynamics/b2Island.cpp \
    ../../libs/Box2D/Dynamics/b2World.cpp \
    ../../libs/Box2D/Dynamics/b2WorldCallbacks.cpp \
    ../../libs/Box2D/Rope/b2Rope.cpp \
    ../../libs/easyloggingpp/easylogging++.cpp \
    ../../libs/imgui/imgui.cpp \
    ../../libs/imgui/imgui_demo.cpp \
    ../../libs/imgui/imgui_draw.cpp \
    ../../libs/pugixml/pugixml.cpp

HEADERS += \
        mainwindow.h \
    StormOpenGlWidget.h \
    SceneObjectsTreeWidget.h \
    StormQtHelperFunctions.h \
    SNumberLineEdit.h \
    propertyWidgets/SWidgetPropertyFloat.h \
    propertyWidgets/SWidgetPropertyVec2.h \
    propertyWidgets/SWidgetPropertyResource.h \
    SWidgetProperty.h \
    componentWidgets/SWidgetComStaticTexture.h \
    SWidgetComponent.h \
    SResourceLineEdit.h \
    SceneObjectTreeWidgetItem.h \
    propertyWidgets/SWidgetPropertyColor.h \
    componentWidgets/SWidgetComTransform.h \
    propertyWidgets/SWidgetPropertyBoolean.h \
    componentWidgets/SWidgetComScript.h \
    selectprefab.h \
    componentWidgets/SWidgetComScript.h \
    componentWidgets/SWidgetComTransform.h \
    propertyWidgets/SWidgetPropertyBoolean.h \
    propertyWidgets/SWidgetPropertyColor.h \
    propertyWidgets/SWidgetPropertyFloat.h \
    propertyWidgets/SWidgetPropertyResource.h \
    propertyWidgets/SWidgetPropertyVec2.h \
    mainwindow.h \
    SceneObjectsTreeWidget.h \
    SceneObjectTreeWidgetItem.h \
    selectprefab.h \
    SNumberLineEdit.h \
    SResourceLineEdit.h \
    StormOpenGlWidget.h \
    StormQtHelperFunctions.h \
    SWidgetComponent.h \
    SWidgetProperty.h \
    ../../src/core/graphics/StormRenderer.h \
    ../../src/core/graphics/StormShader.h \
    ../../src/core/graphics/StormTexture.h \
    ../../src/core/graphics/StormVideoDriver.h \
    ../../src/core/platforms/StormInputManager.h \
    ../../src/core/platforms/StormKeys.h \
    ../../src/core/platforms/StormPlatform.h \
    ../../src/core/platforms/StormPlatformQt.h \
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
    ../../src/core/utils/Geometry.h \
    ../../src/core/utils/Plane.h \
    ../../src/core/utils/SObservable.h \
    ../../src/core/utils/StormIntrusivePtr.h \
    ../../src/core/utils/StormMiscTools.h \
    ../../src/core/utils/StormObjectPool.h \
    ../../src/core/utils/StormRefCounter.h \
    ../../src/core/utils/StormVertex.h \
    ../../src/core/StormCommon.h \
    ../../src/scene/components/luaScript/SLuaBinders.h \
    ../../src/scene/components/luaScript/SSceneComLuaScript.h \
    ../../src/scene/components/luaScript/SSceneSystemLuaScript.h \
    ../../src/scene/components/physics/SSceneComPhysics.h \
    ../../src/scene/components/physics/SSceneSystemPhysics.h \
    ../../src/scene/components/sprite/SSceneComSprite.h \
    ../../src/scene/components/sprite/SSceneSystemSprite.h \
    ../../src/scene/components/transform/SSceneComTransform.h \
    ../../src/scene/components/transform/SSceneSystemTransform.h \
    ../../src/scene/SSceneComponent.h \
    ../../src/scene/SSceneComponentSystem.h \
    ../../src/scene/SSceneManager.h \
    ../../src/scene/SSceneObjectEventTypes.h \
    ../../src/scene/SScene.h \
    ../../src/scene/SSceneObject.h \
    ../../src/SEngineModuleFactory.h \
    ../../src/StormDebug.h \
    ../../src/StormEngine.h \
    ../../src/StormTextureManager.h \
    ../../libs/Box2D/Collision/Shapes/b2ChainShape.h \
    ../../libs/Box2D/Collision/Shapes/b2CircleShape.h \
    ../../libs/Box2D/Collision/Shapes/b2EdgeShape.h \
    ../../libs/Box2D/Collision/Shapes/b2PolygonShape.h \
    ../../libs/Box2D/Collision/Shapes/b2Shape.h \
    ../../libs/Box2D/Collision/b2BroadPhase.h \
    ../../libs/Box2D/Collision/b2Collision.h \
    ../../libs/Box2D/Collision/b2Distance.h \
    ../../libs/Box2D/Collision/b2DynamicTree.h \
    ../../libs/Box2D/Collision/b2TimeOfImpact.h \
    ../../libs/Box2D/Common/b2BlockAllocator.h \
    ../../libs/Box2D/Common/b2Draw.h \
    ../../libs/Box2D/Common/b2GrowableStack.h \
    ../../libs/Box2D/Common/b2Math.h \
    ../../libs/Box2D/Common/b2Settings.h \
    ../../libs/Box2D/Common/b2StackAllocator.h \
    ../../libs/Box2D/Common/b2Timer.h \
    ../../libs/Box2D/Dynamics/Contacts/b2ChainAndCircleContact.h \
    ../../libs/Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.h \
    ../../libs/Box2D/Dynamics/Contacts/b2CircleContact.h \
    ../../libs/Box2D/Dynamics/Contacts/b2Contact.h \
    ../../libs/Box2D/Dynamics/Contacts/b2ContactSolver.h \
    ../../libs/Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.h \
    ../../libs/Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.h \
    ../../libs/Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h \
    ../../libs/Box2D/Dynamics/Contacts/b2PolygonContact.h \
    ../../libs/Box2D/Dynamics/Joints/b2DistanceJoint.h \
    ../../libs/Box2D/Dynamics/Joints/b2FrictionJoint.h \
    ../../libs/Box2D/Dynamics/Joints/b2GearJoint.h \
    ../../libs/Box2D/Dynamics/Joints/b2Joint.h \
    ../../libs/Box2D/Dynamics/Joints/b2MotorJoint.h \
    ../../libs/Box2D/Dynamics/Joints/b2MouseJoint.h \
    ../../libs/Box2D/Dynamics/Joints/b2PrismaticJoint.h \
    ../../libs/Box2D/Dynamics/Joints/b2PulleyJoint.h \
    ../../libs/Box2D/Dynamics/Joints/b2RevoluteJoint.h \
    ../../libs/Box2D/Dynamics/Joints/b2RopeJoint.h \
    ../../libs/Box2D/Dynamics/Joints/b2WeldJoint.h \
    ../../libs/Box2D/Dynamics/Joints/b2WheelJoint.h \
    ../../libs/Box2D/Dynamics/b2Body.h \
    ../../libs/Box2D/Dynamics/b2ContactManager.h \
    ../../libs/Box2D/Dynamics/b2Fixture.h \
    ../../libs/Box2D/Dynamics/b2Island.h \
    ../../libs/Box2D/Dynamics/b2TimeStep.h \
    ../../libs/Box2D/Dynamics/b2World.h \
    ../../libs/Box2D/Dynamics/b2WorldCallbacks.h \
    ../../libs/Box2D/Rope/b2Rope.h \
    ../../libs/Box2D/Box2D.h \
    ../../libs/easyloggingpp/easylogging++.h \
    ../../libs/imgui/imconfig.h \
    ../../libs/imgui/imgui.h \
    ../../libs/imgui/imgui_internal.h \
    ../../libs/imgui/stb_rect_pack.h \
    ../../libs/imgui/stb_textedit.h \
    ../../libs/imgui/stb_truetype.h \
    ../../libs/pugixml/pugiconfig.hpp \
    ../../libs/pugixml/pugixml.hpp \
    ../../libs/sol2/sol/compatibility/5.0.0.h \
    ../../libs/sol2/sol/compatibility/5.1.0.h \
    ../../libs/sol2/sol/compatibility/5.2.0.h \
    ../../libs/sol2/sol/compatibility/5.x.x.h \
    ../../libs/sol2/sol/compatibility/version.hpp \
    ../../libs/sol2/sol/as_args.hpp \
    ../../libs/sol2/sol/as_returns.hpp \
    ../../libs/sol2/sol/bind_traits.hpp \
    ../../libs/sol2/sol/call.hpp \
    ../../libs/sol2/sol/compatibility.hpp \
    ../../libs/sol2/sol/container_traits.hpp \
    ../../libs/sol2/sol/container_usertype_metatable.hpp \
    ../../libs/sol2/sol/coroutine.hpp \
    ../../libs/sol2/sol/debug.hpp \
    ../../libs/sol2/sol/demangle.hpp \
    ../../libs/sol2/sol/deprecate.hpp \
    ../../libs/sol2/sol/environment.hpp \
    ../../libs/sol2/sol/error.hpp \
    ../../libs/sol2/sol/feature_test.hpp \
    ../../libs/sol2/sol/filters.hpp \
    ../../libs/sol2/sol/forward.hpp \
    ../../libs/sol2/sol/forward_detail.hpp \
    ../../libs/sol2/sol/function.hpp \
    ../../libs/sol2/sol/function_result.hpp \
    ../../libs/sol2/sol/function_types.hpp \
    ../../libs/sol2/sol/function_types_core.hpp \
    ../../libs/sol2/sol/function_types_overloaded.hpp \
    ../../libs/sol2/sol/function_types_stateful.hpp \
    ../../libs/sol2/sol/function_types_stateless.hpp \
    ../../libs/sol2/sol/function_types_templated.hpp \
    ../../libs/sol2/sol/in_place.hpp \
    ../../libs/sol2/sol/inheritance.hpp \
    ../../libs/sol2/sol/load_result.hpp \
    ../../libs/sol2/sol/object.hpp \
    ../../libs/sol2/sol/object_base.hpp \
    ../../libs/sol2/sol/optional.hpp \
    ../../libs/sol2/sol/optional_implementation.hpp \
    ../../libs/sol2/sol/overload.hpp \
    ../../libs/sol2/sol/property.hpp \
    ../../libs/sol2/sol/protect.hpp \
    ../../libs/sol2/sol/protected_function.hpp \
    ../../libs/sol2/sol/protected_function_result.hpp \
    ../../libs/sol2/sol/proxy.hpp \
    ../../libs/sol2/sol/proxy_base.hpp \
    ../../libs/sol2/sol/raii.hpp \
    ../../libs/sol2/sol/reference.hpp \
    ../../libs/sol2/sol/resolve.hpp \
    ../../libs/sol2/sol/simple_usertype_metatable.hpp \
    ../../libs/sol2/sol/stack.hpp \
    ../../libs/sol2/sol/stack_check.hpp \
    ../../libs/sol2/sol/stack_check_get.hpp \
    ../../libs/sol2/sol/stack_core.hpp \
    ../../libs/sol2/sol/stack_field.hpp \
    ../../libs/sol2/sol/stack_get.hpp \
    ../../libs/sol2/sol/stack_guard.hpp \
    ../../libs/sol2/sol/stack_pop.hpp \
    ../../libs/sol2/sol/stack_probe.hpp \
    ../../libs/sol2/sol/stack_proxy.hpp \
    ../../libs/sol2/sol/stack_push.hpp \
    ../../libs/sol2/sol/stack_reference.hpp \
    ../../libs/sol2/sol/state.hpp \
    ../../libs/sol2/sol/state_view.hpp \
    ../../libs/sol2/sol/string_shim.hpp \
    ../../libs/sol2/sol/table.hpp \
    ../../libs/sol2/sol/table_core.hpp \
    ../../libs/sol2/sol/table_iterator.hpp \
    ../../libs/sol2/sol/thread.hpp \
    ../../libs/sol2/sol/tie.hpp \
    ../../libs/sol2/sol/traits.hpp \
    ../../libs/sol2/sol/tuple.hpp \
    ../../libs/sol2/sol/types.hpp \
    ../../libs/sol2/sol/unsafe_function.hpp \
    ../../libs/sol2/sol/userdata.hpp \
    ../../libs/sol2/sol/usertype.hpp \
    ../../libs/sol2/sol/usertype_core.hpp \
    ../../libs/sol2/sol/usertype_metatable.hpp \
    ../../libs/sol2/sol/usertype_traits.hpp \
    ../../libs/sol2/sol/variadic_args.hpp \
    ../../libs/sol2/sol/variadic_results.hpp \
    ../../libs/sol2/sol/wrapper.hpp \
    ../../libs/sol2/sol.hpp

FORMS += \
        mainwindow.ui \
    selectprefab.ui

DISTFILES +=
