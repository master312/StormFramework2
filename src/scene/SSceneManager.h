#pragma once
#include <map>
#include "StormScene.h"

class StormRenderer;

class SSceneManager {
public:
    SSceneManager();
    ~SSceneManager();

    /* Loads single scene file into memory.
     * If scene is already loaded, it will be reloaded.
     * reloadActive is set to true, scene will be reloaded even if its currently active
     * Returns pointer to just loaded scene, or nullptr on error. */
    StormScene* loadScene(const std::string& filename, bool reloadActive = false);

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
