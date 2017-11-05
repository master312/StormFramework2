#pragma once
#include "../utils/math/Rect.h"
#include "../utils/math/Color.h"

/* Video driver manager. Currently works with GLES
 * REMAINDER: If porting to some other platform (non-glsl) this class needs to
 * be used as base class for all other platforms */

class StormVideoDriver {
public:
    StormVideoDriver();
    virtual ~StormVideoDriver();
    
    /* Initializes GLES, viewport and other view stuff. 
     * GL_Context must already be created by StormWindow class before initialization.
     * Return < 0 on error. */
    int initialize(Point windowSize);
    
    /* Begin rendering */
    void begin();
    void clear();
    
    /* Sets screen clear color */
    void setClearColor(Color color);

    /* Sets size of virtual coordinate system that will be fit on the game window */
    void setVirtualViewSize(float width, float height);
    void setVirtualViewSize(const Vector2 size);

    Vector2 getVirtualViewSize() const;
    
    /* Gets how many pixels are representing one meter */
    float getPixelsMeterScale() const;

    /* Sets how many pixels are representing one meter */
    void setPixelsMeterScale(float scale);

private:
    Color _ClearColor;
    
    /* Virtual drawing space size */
    Vector2 _VirtualViewSize;

    /* Number of pixels per meter. */
    float _PixelsPerMeter;
};
