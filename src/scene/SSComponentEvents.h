#pragma once
#include "StormCommon.h"
#include "utils/SEventDispatcher.h"

typedef enum {
    S_EVENT_UNDEFINED = 0,

    /* Fired from SSceneComTransform when transformation has been updated */
    S_EVENT_TRANSFORM_UPDATED = S_GENERATE_EVENT_ID('C', 'T', 'U'),

    /* Fired from parent's transform component to notify children that 
     * parent transformation has been changed and that they need to update transform too */
    S_EVENT_PARENT_TRANSFORM_UPDATED = S_GENERATE_EVENT_ID('P', 'T', 'U'),

    /* Fired from SSceneObject it self when parent changes */
    S_EVENT_PARENT_CHANGED = S_GENERATE_EVENT_ID('O', 'P', 'C'),

} SSComponentEvents;