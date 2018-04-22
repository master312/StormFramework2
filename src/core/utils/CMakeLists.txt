set(PATH_PREFIX ${STORM_SRC_DIR}/core/utils)


set(STORM_CORE_UTILS_FILES
    ${PATH_PREFIX}/Geometry.h
    ${PATH_PREFIX}/Geometry.cpp
    ${PATH_PREFIX}/Plane.h
    ${PATH_PREFIX}/Plane.cpp
    ${PATH_PREFIX}/SEventListener.h
    ${PATH_PREFIX}/SNotificationCenter.h
    ${PATH_PREFIX}/StormIntrusivePtr.h
    ${PATH_PREFIX}/StormMiscTools.h
    ${PATH_PREFIX}/StormMiscTools.cpp
    ${PATH_PREFIX}/StormObjectPool.h
    ${PATH_PREFIX}/StormRefCounter.h
    ${PATH_PREFIX}/StormVertex.h
    ${PATH_PREFIX}/math/Color.h
    ${PATH_PREFIX}/math/Matrix.h
    ${PATH_PREFIX}/math/Rect.h
    ${PATH_PREFIX}/math/ScalarMath.h
    ${PATH_PREFIX}/math/TrigonometryMath.h
    ${PATH_PREFIX}/math/Vector2.h
    ${PATH_PREFIX}/math/Vector3.h
    ${PATH_PREFIX}/math/Vector4.h
    )


source_group(StormCoreUtils FILES ${STORM_CORE_UTILS_FILES})
unset(PATH_PREFIX)