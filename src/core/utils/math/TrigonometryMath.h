#pragma once
#include "ScalarMath.h"
#include "Vector2.h"

namespace StormTrigonometryMath {
    
    inline void rotatePointAround(Vector2& point, const Vector2& origin, float angle) {
        float sin = StormScalarMath::sin(angle);
        float cos = StormScalarMath::cos(angle);
    
        Vector2 tmpPoint = point;
        point.x = (tmpPoint.x * cos - tmpPoint.y * sin) + origin.x;
        point.y = (tmpPoint.y * cos + tmpPoint.x * sin) + origin.y;
    }
    
} /* namespace TrigonometryMath */
