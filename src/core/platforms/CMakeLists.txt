set(PATH_PREFIX src/core/platforms)

set(STORM_CORE_PLATFORM_FILES
    ${PATH_PREFIX}/StormPlatform.h
    ${PATH_PREFIX}/StormPlatform.cpp
    ${PATH_PREFIX}/StormInputManager.h
    ${PATH_PREFIX}/StormInputManager.cpp
    ${PATH_PREFIX}/StormKeys.h
    ${PATH_PREFIX}/StormPlatformSDL2.h
    ${PATH_PREFIX}/StormPlatformSDL2.cpp
    )


source_group(StormCorePlatform FILES ${STORM_CORE_PLATFORM_FILES})
unset(PATH_PREFIX)