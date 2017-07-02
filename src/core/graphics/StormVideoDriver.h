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
    
    /* This viewport will be used when calling begin() without viewport specified */
    void setDefaultViewport(const Rect& viewport);
    /* Sets screen clear color */
    void setClearColor(Color color);

private:
    Color _ClearColor;
};
