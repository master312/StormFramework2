#pragma once
#include <string>
#include <functional>
#include "StormInputManager.h"
#include "../utils/math/Vector2.h"

/* Base class for platform related stuff, like
 * window creation, and event handling. */

enum StormPlatformType {
    STORM_PLATFORM_NONE,
    STORM_PLATFORM_SDL2,
    STORM_PLATFORM_IOS,
    STORM_PLATFORM_ANDROID,
    STORM_PLATFORM_QT
};

/* Struct holds basic informations about game window */
struct StormWindowSettings {
    int width;
    int height;
    bool isFullscreen;
    bool useVsync;
    std::string title;

    StormWindowSettings() : width(0), height(0), isFullscreen(false), useVsync(false) { }
    StormWindowSettings(int w, int h, bool fs, const std::string& wTitle, bool vsync = false) : 
            width(w), height(h), isFullscreen(fs), title(wTitle), useVsync(vsync) { }
    
    const Point getSizePoint() {
        return Point(width, height);
    }
    const Vector2 getSizeVec2() {
        return Vector2((float)width, (float)height);
    }
};

enum StormWindowEventType {
    STORM_EVENT_WINDOW_RESIZED,
    STORM_EVENT_WINDOW_HIDDEN,
    STORM_EVENT_WINDOW_SHOWN,
    STORM_EVENT_WINDOW_MOVED,
    STORM_EVENT_WINDOW_FOCUS_GAINED,
    STORM_EVENT_WINDOW_FOCUS_LOST
    /* etc.... */
};

class StormPlatform {
public:
    StormPlatform();
    virtual ~StormPlatform();

    /* Initialize platform related apis. 
     * Returns < 0 on error */
    virtual int initialize() = 0;
    
    /* Deinitialize all platfrom related stuff and free memory */
    virtual void deinitialize() = 0;

    /* Creates actual game window. 
     * Returns < 0 on  error. */
    virtual int createWindow(StormWindowSettings settings) = 0;
    
    /* Swaps back buffet with window buffer, to display content on screen */
    virtual void windowSwapBuffers() = 0;
    
    /* Process all windows events and map them to input manager */
    virtual void processEvents() = 0;

    /* Returns time since program start in milliseconds (ms) */
    virtual uint32_t getTimeMs() = 0;
    
    /* Returns time since program start in nanosecounds (ns) */
    virtual uint64_t getTimeNs() = 0;

    /* Sets main ticking function. */
    void setMainTickingFunction(std::function<void(float)> tickingFun);

    /* Sets listener method for window events. (like window resize, minimize, etc...) */
    void setWindowEventListener(std::function<void(StormWindowEventType)> listener);

    /* Starts main loop ticking. This method will block until program ends. */
    virtual void startMainLoop();

    /* Returns true if quit event has been received from OS */
    bool shouldQuit();
    
    /* Sets @_ShouldQuit to true */
    void quit();

    StormPlatformType getType() const;
    StormWindowSettings getWindowSettings() const;
    StormInputManager* getInputManager();

protected:
    bool _IsInitialized;
    StormPlatformType _PlatformType;
    StormWindowSettings _WindowSettings;
    
    StormInputManager* _InputManager;
    bool _ShouldQuit;

    /* Main ticking function that will be called every tick */
    std::function<void(float)> _MainTickingFunction;

    /* Method that will be called when window state changes. */
    std::function<void(StormWindowEventType)> _WindowEventListener;
};
