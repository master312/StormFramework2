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
    virtual uint32_t getRunningTime();

private:
    SDL_Window* _Window;
};