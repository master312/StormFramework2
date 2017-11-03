#pragma once
#include "math/Vector2.h"
#include "math/Color.h"

struct StormVertex {
    Vector2 position;
    Vector2 uv;
    Color color;

    StormVertex() { }
    StormVertex(Vector2 _position, Vector2 _uv) : position(_position), uv(_uv) { }
};