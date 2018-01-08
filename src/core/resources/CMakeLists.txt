set(PATH_PREFIX src/core/resources)


set(STORM_CORE_RESOURCE_FILES
    ${PATH_PREFIX}/StormFileSystem.h
    ${PATH_PREFIX}/StormFileSystem.cpp
    ${PATH_PREFIX}/StormResourceFile.h
    ${PATH_PREFIX}/StormResourceFile.cpp
    )


source_group(StormCoreResources FILES ${STORM_CORE_RESOURCE_FILES})
unset(PATH_PREFIX)