set(PATH_PREFIX ${STORM_SRC_DIR}/scene/components)

set(STORM_ENGINE_SCENE_COMPONENTS_FILES
    ${PATH_PREFIX}/luaScript/SLuaBindings.h
    ${PATH_PREFIX}/luaScript/SLuaBindings.cpp
    ${PATH_PREFIX}/luaScript/SComLuaScript.h
    ${PATH_PREFIX}/luaScript/SComLuaScript.cpp
    ${PATH_PREFIX}/luaScript/SSystemLuaScript.h
    ${PATH_PREFIX}/luaScript/SSystemLuaScript.cpp

    ${PATH_PREFIX}/physics/SComPhysics.h
    ${PATH_PREFIX}/physics/SComPhysics.cpp
    ${PATH_PREFIX}/physics/SSystemPhysics.h
    ${PATH_PREFIX}/physics/SSystemPhysics.cpp

    ${PATH_PREFIX}/sprite/SComSprite.h
    ${PATH_PREFIX}/sprite/SComSprite.cpp
    ${PATH_PREFIX}/sprite/SSystemSprite.h
    ${PATH_PREFIX}/sprite/SSystemSprite.cpp

    ${PATH_PREFIX}/transform/SComTransform.h
    ${PATH_PREFIX}/transform/SComTransform.cpp
    ${PATH_PREFIX}/transform/SSystemTransform.h
    ${PATH_PREFIX}/transform/SSystemTransform.cpp
    )

source_group(StormEngineSceneComponents FILES ${STORM_ENGINE_SCENE_COMPONENTS_FILES})
unset(PATH_PREFIX)