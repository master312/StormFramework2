set(PATH_PREFIX src/scene)

set(STORM_ENGINE_SCENE_FILES
        ${PATH_PREFIX}/StormScene.h
        ${PATH_PREFIX}/StormScene.cpp
        ${PATH_PREFIX}/SSceneManager.h
        ${PATH_PREFIX}/SSceneManager.cpp
        ${PATH_PREFIX}/StormSceneObject.h
        ${PATH_PREFIX}/StormSceneObject.cpp
        ${PATH_PREFIX}/SSceneComponent.h
        ${PATH_PREFIX}/SSceneComponent.cpp
        ${PATH_PREFIX}/SSceneComponentSystem.h
        ${PATH_PREFIX}/SSceneComponentSystem.cpp
        ${PATH_PREFIX}/SSceneObjectEventTypes.h
    )

source_group(StormEngineScene FILES ${STORM_ENGINE_SCENE_FILES})
unset(PATH_PREFIX)