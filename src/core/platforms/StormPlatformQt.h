#ifdef STORM_BUILD_PLATFORM_QT
#pragma once
#include "StormPlatform.h"
#include "../StormCommon.h"

class QTime;

class StormPlatformQt : public StormPlatform {
public:
    StormPlatformQt();
    virtual ~StormPlatformQt();

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

    /* Called from QT timer, to tick engine */
    void mainTick();

    /* Called from QT widget when widget gets resized */
    void handleWidgetResize(int width, int height);

    /* Sence we tick main function from QT timer,
     * we dont need startMainLoop() method. */
    virtual void startMainLoop() { }

private:
    /* Used for determening app uptime */
    QTime* _Time;

    /* Used for calculating delta time */
    uint32_t _LastTickTime;

    float _AverageFps;
    float _AverageFpsCounter;
};

#endif
