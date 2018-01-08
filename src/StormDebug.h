#pragma once
#ifndef PRODUCTION
#include <iostream>

/* Class containing all debug stuff */

class StormDebug {
public:
    virtual ~StormDebug();

    static StormDebug* instance();
    
    void processInput();

    static void setShouldTickLogic(bool value);
    static void setShouldTickPhysics(bool value);

    static bool shouldTickLogic();
    static bool shouldTickPhysics();

private:
    StormDebug();


    bool _ShouldTickLogic;
    bool _ShouldTickPhysics;


    int _SceneSavesCount;
    std::string _LastSavedSceneName;
    void saveCurrentScene();
    void loadMostRecentScene();
};


#endif
