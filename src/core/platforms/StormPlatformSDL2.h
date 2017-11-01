#ifdef STORM_BUILD_PLATFORM_SDL2
#pragma once
#include "StormPlatform.h"
#include <SDL2/SDL.h>

/* SDL2 platform stuff */

class StormPlatformSDL2 : public StormPlatform {
public:
    StormPlatformSDL2();
    virtual ~StormPlatformSDL2();

    /* Initialize platform related apis. 
     * Returns < 0 on error */
    virtual int initialize();

    /* Deinitialize all platfrom related stuff and free memory */
    virtual void deinitialize();

    /* Creates actual game window. 
     * Returns < 0 on  error. */
    virtual int createWindow(StormWindowSettings settings);

    /* Swaps back buffet with window buffer, to display content on screen */
    virtual void windowSwapBuffers();

    /* Process all windows events and map them to input manager */
    virtual void processEvents();

    /* Returns time since program start in milliseconds (ms) */
    virtual uint32_t getTimeMs();

    /* Returns time since program start in nanosecounds (ns) */
    virtual uint64_t getTimeNs();

private:
    SDL_Window* _Window;
    
    uint32_t _StartTimeMs;
    uint64_t _StartTimeNs;
    
    /* Gets time in nanosecounds converted to uint64_t, using std::chrono */
    uint64_t getNanoTime();

    void handleWindowEvent(SDL_Event& event);
};

#endif /* STORM_BUILD_PLATFORM_QT */
