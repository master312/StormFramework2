set(CORE_PATH_PREFIX ${STORM_SRC_DIR}/core)

include(${CORE_PATH_PREFIX}/platforms/CMakeLists.txt)
include(${CORE_PATH_PREFIX}/resources/CMakeLists.txt)
include(${CORE_PATH_PREFIX}/graphics/CMakeLists.txt)
include(${CORE_PATH_PREFIX}/utils/CMakeLists.txt)

set(STORM_CORE_FILES
    ${CORE_PATH_PREFIX}/StormCommon.h
    ${CORE_PATH_PREFIX}/StormModuleBase.h

    ${STORM_CORE_PLATFORM_FILES}
    ${STORM_CORE_RESOURCE_FILES}
    ${STORM_CORE_GRAPHICS_FILES}
    ${STORM_CORE_UTILS_FILES}
    )

source_group(StormCore FILES ${STORM_CORE_FILES})
unset(PATH_PREFIX)