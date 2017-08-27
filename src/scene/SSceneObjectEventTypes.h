#pragma once

typedef enum SSceneObjectEventType {
    S_OBSERVER_EVENT_UNDEFINED = 0,

    /* Fired from SSceneComTransform when transformation has been updated */
    S_OBSERVER_EVENT_TRANSFORM_UPDATED,
    /* Fired from parent's transform component to notify children that 
     * parent transformation has been changed and that they need to update transform too */
    S_OBSERVER_EVENT_PARENT_TRANSFORM_UPDATED,


    /* Fired from StormSceneObject it self when parent changes */
    S_OBSERVER_EVENT_PARENT_CHANGED,
};