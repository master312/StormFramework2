#pragma once
#include <cstdint>

struct Color {
    uint8_t r, g, b, a;
    
    Color() : r(255), g(255), b(255), a(255) { }
    Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 255) :
        r(_r), g(_g), b(_b), a(_a) { }

    inline void set(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 255) {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    inline void setFloat(float _r, float _g, float _b, float _a) {
        r = (uint8_t)(_r * 255.0f);
        g = (uint8_t)(_g * 255.0f);
        b = (uint8_t)(_b * 255.0f);
        a = (uint8_t)(_a * 255.0f);
    }

    /* Converts color 0-255 to 0.0-1.0 range */
    inline float floatR() { return (float)r / 255.0f; }
    inline float floatG() { return (float)g / 255.0f; }
    inline float floatB() { return (float)b / 255.0f; }
    inline float floatA() { return (float)a / 255.0f; }

    inline bool operator==(const Color& color) { return color.r == r && color.g == g && color.b == b && color.a == a; }
};