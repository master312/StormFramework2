set(SOURCES_PATH_PREFIX ${STORM_SRC_DIR})

include(${SOURCES_PATH_PREFIX}/core/CMakeLists.txt)
include(${SOURCES_PATH_PREFIX}/scene/CMakeLists.txt)
include(${SOURCES_PATH_PREFIX}/scene/components/CMakeLists.txt)

set(STORM_ENGINE_SOURCE_FILES
    ${STORM_CORE_FILES}

    ${SOURCES_PATH_PREFIX}/StormEngine.h
    ${SOURCES_PATH_PREFIX}/StormEngine.cpp
    ${SOURCES_PATH_PREFIX}/SEngineModuleFactory.h
    ${SOURCES_PATH_PREFIX}/SEngineModuleFactory.cpp
    ${SOURCES_PATH_PREFIX}/StormDebug.h     #TODO: dont include debug stuff on production mode
    ${SOURCES_PATH_PREFIX}/StormDebug.cpp
    ${SOURCES_PATH_PREFIX}/STextureManager.h
    ${SOURCES_PATH_PREFIX}/STextureManager.cpp

    ${SOURCES_PATH_PREFIX}/lua/SLuaSystem.h
    ${SOURCES_PATH_PREFIX}/lua/SLuaSystem.cpp

    ${SOURCES_PATH_PREFIX}/lua/SLuaBindings.h
    ${SOURCES_PATH_PREFIX}/lua/SLuaBindings.cpp

    # Scene and component files
    ${STORM_ENGINE_SCENE_FILES}
    ${STORM_ENGINE_SCENE_COMPONENTS_FILES}
    )


### Set include paths ###
# Include src/core/ directory
include_directories(${CORE_PATH_PREFIX})
include_directories(${SOURCES_PATH_PREFIX})

source_group(StormEngine FILES ${STORM_ENGINE_SOURCE_FILES})
unset(SOURCES_PATH_PREFIX)