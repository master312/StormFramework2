#pragma once

typedef enum SSceneObjectEventType {
    S_OBSERVER_EVENT_UNDEFINED = 0,

    /* Fired from SSceneComTransform when transformation has been updated */
    S_OBSERVER_EVENT_TRANSFORM_UPDATED = 1,
};