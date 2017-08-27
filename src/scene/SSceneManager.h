#pragma once
#include <map>
#include "StormScene.h"

class StormRenderer;

class SSceneManager {
public:
    SSceneManager();
    ~SSceneManager();

    /* Loads single scene file into memory.
     * Returns < 0 on failure */
    int loadScene(const std::string& filename);

    /* Switches to specified scene. 
     * @sceneName is name of scene specified in scene file 
     * (not the name of scene file it self) */
    void switchScene(const std::string& sceneName);

    /* Returns currently active scene, or nullptr if no scene is active */
    StormScene* getActiveScene();

    /* Renders currently active scene */
    void render(StormRenderer* renderer);

    /* Update (ticks) currently active scene(s) */
    void tick(float deltaTime);

private:
    /* Map of all loaded scenes, indexed by their names */
    std::map<std::string, StormScene*> _LoadedScenes;
    
    /* Currently active scene */
    StormScene* _ActiveScene;
};