#pragma once
#include <map>
#include "SScene.h"
#include "core/StormModuleBase.h"
#include "utils/SEventDispatcher.h"

class StormRenderer;


/* Define scene manager event structure */
namespace SEventDispatcher {
    struct SceneChangeEvent : public Event {
        enum Types {
            ABOUT_TO_CHANGE = S_GENERATE_EVENT_ID('s', 'a', 'c'),
            CHANGED = S_GENERATE_EVENT_ID('s', 'c', 'c'),
        };

        SScene* scene;
        SceneChangeEvent(int32_t type) : Event(type), scene(nullptr) { }
    };
};

class SSceneManager : public StormModuleBase {
public:
    SSceneManager();
    virtual ~SSceneManager();

    /* Loads single scene file into memory.
     * If scene is already loaded, it will be reloaded.
     * reloadActive is set to true, scene will be reloaded even if its currently active
     * Returns pointer to just loaded scene, or nullptr on error. */
    SScene* loadScene(const std::string& filename, bool reloadActive = false);

    /* Switches to specified scene. 
     * @sceneName is name of scene specified in scene file 
     * (not the name of scene file it self) */
    void switchScene(const std::string& sceneName);

    /* Switches to specified scene.
     * @scene is pointer to scene object.
     * @scene must already be loaded in scene manager, using @loadScene method. */
    void switchScene(const SScene* scene);

    /* Unloads scene.
     * INFO: This method will safely remove scene from @_LoadedScenes map */
    void unloadScene(const std::string& name);

    /* Unloads scene.
     * INFO: This method will safely remove scene from @_LoadedScenes map */
    void unloadScene(const SScene* scene);

    /* Returns currently active scene, or nullptr if no scene is active */
    SScene* getActiveScene();

    /* Renders currently active scene */
    void render(StormRenderer* renderer);

    /* Update (ticks) currently active scene(s) */
    void tick(float deltaTime);

private:
    /* Map of all loaded scenes, indexed by their names */
    std::map<std::string, SScene*> _LoadedScenes;
    
    /* Currently active scene.
     * WARNING: Never change manually! Use @setActiveSceneInternal */
    SScene* _ActiveScene;


    void setActiveSceneInternal(SScene* activeScene);
};
