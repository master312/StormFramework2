set(PATH_PREFIX ${STORM_SRC_DIR}/core/graphics)


set(STORM_CORE_GRAPHICS_FILES
    ${PATH_PREFIX}/StormRenderer.h
    ${PATH_PREFIX}/StormRenderer.cpp
    ${PATH_PREFIX}/StormShader.h
    ${PATH_PREFIX}/StormShader.cpp
    ${PATH_PREFIX}/StormTexture.h
    ${PATH_PREFIX}/StormTexture.cpp
    ${PATH_PREFIX}/StormVideoDriver.h
    ${PATH_PREFIX}/StormVideoDriver.cpp
    )


source_group(StormCoreGraphics FILES ${STORM_CORE_GRAPHICS_FILES})
unset(PATH_PREFIX)