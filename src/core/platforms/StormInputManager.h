#pragma once
#include "StormKeys.h"
#include "../utils/math/Vector2.h"

/*
    TODO: 
        - Register callbacks for specified input events
        - Touch support
        - Rethink this class ?
*/

class StormInputManager {

    /* Number of maximum key that can be pressed at the same time */
    static const int MAXIMUM_KEYS_PRESSED = 5;
    /* Maximum number of pointers (touches or mouse buttons count) that can be handled */
    static const int MAXIMUM_POINTERS_PRESSED = 5;

public:
    StormInputManager();
    virtual ~StormInputManager();

    /* Should once per frame before using process event functions. */
    void begin();

    /* Sets size of actual program window. 
     * If not set, pointer position will be invalid */
    void setRealWindowSize(Vector2 realWindowSize);

    /* Calculates pointer scaling based on size of virtual window size (perspective)
     * WARNING: Real window size MUST be set first (using @setRealWindowSize)
     *          If not set, pointer position might be invalid */
    void calculatePointerScaling(Vector2 virtualViewSize);

    /* Returns true if @key is currently pressed. */
    bool isKeyDown(StormKey key);
    
    /* Returns true if @key was pressed in this frame */
    bool isKeyPressed(StormKey key);

    /* Returns true if mouse button or touch with index @pointerIndex is pressed */
    bool isPointerDown(int pointerIndex = S_POINTER_LEFT);
    
    /* @pointerIndex is used for multitouch.  
     * Returns pointer position. */
    Vector2 getPointerPosition(int pointerIndex = S_POINTER_LEFT);

    /* Method used to map actual events. Called from StormPlatform class */
    void processKeyEvent(bool isDown, StormKey key);
    void processPointerMotion(int pointerIndex, int xPos, int yPos);
    void processPointerButton(int pointerIndex, bool isDown);
    

    /* Returns matching string for @key. */
    static const char* keyToString(const StormKey key);

private:
    /* Array of all keys that are currently pressed(in down state) */
    StormKey _KeysDown[MAXIMUM_KEYS_PRESSED];

    /* Array of all key that ware pressed in this frame */
    StormKey _KeysPressed[MAXIMUM_KEYS_PRESSED];

    /* Array of positions of all pointers */
    Vector2 _PointersPositions[MAXIMUM_POINTERS_PRESSED];

    /* Array of booleans, representing wether pointer is down or not */
    bool _PointersDown[MAXIMUM_POINTERS_PRESSED];

    /* Size of actual window on screen in pixels */
    Vector2 _RealWindowSize;
    
    /* Pointer position multiplier. Calculated in @setVirtualViewSize */
    Vector2 _PointerScalingFactor;

    /* Reset all keys and pointers to default state (released state) */
    void resetAllEvents();
};