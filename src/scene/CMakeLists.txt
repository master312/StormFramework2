set(PATH_PREFIX ${STORM_SRC_DIR}/scene)

set(STORM_ENGINE_SCENE_FILES
        ${PATH_PREFIX}/SScene.h
        ${PATH_PREFIX}/SScene.cpp
        ${PATH_PREFIX}/SSceneManager.h
        ${PATH_PREFIX}/SSceneManager.cpp
        ${PATH_PREFIX}/SSceneObject.h
        ${PATH_PREFIX}/SSceneObject.cpp
        ${PATH_PREFIX}/SSceneComponent.h
        ${PATH_PREFIX}/SSceneComponent.cpp
        ${PATH_PREFIX}/SSceneComponentSystem.h
        ${PATH_PREFIX}/SSceneComponentSystem.cpp
    )

source_group(StormEngineScene FILES ${STORM_ENGINE_SCENE_FILES})
unset(PATH_PREFIX)