#pragma once
#ifndef PRODUCTION
#include "scene/SSceneComponent.h"
#include <iostream>

/* Class containing all debug stuff */

class StormDebug {
public:
    virtual ~StormDebug();

    static StormDebug* instance();
    
    void processInput();

    /* Enable / Disable whole logic tick */
    static void setShouldTickLogic(bool value);

    /* Should logic tick execute */
    static bool shouldTickLogic();


    /* Sets whether some system should be ticked or not */
    static void setSystemTickingEnabled(SSceneComponentType type, bool isEnabled);

    /* Returns true if specified system (@type) should be ticked */
    static bool shouldTickSystem(SSceneComponentType type);

private:
    StormDebug();

    bool _ShouldTickLogic;

    /* List of component systems that should be ticked.
     * If value is set to FALSE system wont be ticked */
    bool _SystemsTickable[S_SCENE_OBJECT_COM_TYPES_COUNT];

    int _SceneSavesCount;
    std::string _LastSavedSceneName;
    void saveCurrentScene();
    void loadMostRecentScene();
};


#endif
