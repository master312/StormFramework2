#pragma once
#include "StormCommon.h"

enum SSceneObjectFlags {
    ZERO_FLAG = 0,

    /* Set from SSceneObject it self when parent changes */
    PARENT_CHANGED = 1,

    /* Set from SSceneComTransform when transformation has been updated */
    TRANSFORM_UPDATED = 2,

    /* Set from parent's transform component to notify children that
     * parent transformation has been changed and that they need to update transform too */
    PARENT_TRANSFORM_UPDATED = 3,

    FLAGS_MAX,
};