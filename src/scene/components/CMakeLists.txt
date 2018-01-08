set(PATH_PREFIX src/scene/components)

set(STORM_ENGINE_SCENE_COMPONENTS_FILES
    ${PATH_PREFIX}/luaScript/SLuaBinders.h
    ${PATH_PREFIX}/luaScript/SLuaBinders.cpp
    ${PATH_PREFIX}/luaScript/SSceneComLuaScript.h
    ${PATH_PREFIX}/luaScript/SSceneComLuaScript.cpp
    ${PATH_PREFIX}/luaScript/SSceneSystemLuaScript.h
    ${PATH_PREFIX}/luaScript/SSceneSystemLuaScript.cpp

    ${PATH_PREFIX}/physics/SSceneComPhysics.h
    ${PATH_PREFIX}/physics/SSceneComPhysics.cpp
    ${PATH_PREFIX}/physics/SSceneSystemPhysics.h
    ${PATH_PREFIX}/physics/SSceneSystemPhysics.cpp

    ${PATH_PREFIX}/sprite/SSceneComSprite.h
    ${PATH_PREFIX}/sprite/SSceneComSprite.cpp
    ${PATH_PREFIX}/sprite/SSceneSystemSprite.h
    ${PATH_PREFIX}/sprite/SSceneSystemSprite.cpp

    ${PATH_PREFIX}/transform/SSceneComTransform.h
    ${PATH_PREFIX}/transform/SSceneComTransform.cpp
    ${PATH_PREFIX}/transform/SSceneSystemTransform.h
    ${PATH_PREFIX}/transform/SSceneSystemTransform.cpp
    )

source_group(StormEngineSceneComponents FILES ${STORM_ENGINE_SCENE_COMPONENTS_FILES})
unset(PATH_PREFIX)