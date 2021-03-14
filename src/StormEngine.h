#pragma once
#include <typeindex>
#include "utils/SEventDispatcher.h"
#include "platforms/StormPlatform.h"
#include "resources/StormFileSystem.h"
#include "STextureManager.h"

/* Singleton class responsible for initialization and deinitialization of all engine modules */

class StormVideoDriver;
class StormRenderer;
class SSceneManager;
class SScene;
class StormInputManager;
class SLuaSystem;

class StormEngine {
    friend class SEngineModuleFactory;
    
public:
    virtual ~StormEngine();

    /* Gets singleton instance */
    static StormEngine* instance();

    /* Initialize all engine modules. 
     * If there was an error during initialization, if will be logged, and program will crash */
    void initialize(StormPlatformType platformType);

    /* Deinitialize all modules, frees all memory, and ends main loop */
    void deinitialize();

    /* Starts engine main loop. This method will block until program ends */
    void startMainLoop();

    /* Stops engine */
    void quit();

    /**** Often used module methods for easy access ****/
    /* Returns resource file from default filesystem,
     * or nullptr if not found */
    static spStormResourceFile getResource(const std::string& filename);    

    /* Returns pointer input manager module */
    static StormInputManager* getInputManager();

    /* Returns pointer to scene manager module */
    static SSceneManager* getSceneManager();

    /* Returns pointer to lua system module*/
    static SLuaSystem* getLua();

    /* Fires event to global event dispatcher */
    static void fireEvent(SEventDispatcher::Event* event);

    /* Returns pointer to engine's event dispatcher */
    static SEventDispatcher::Dispatcher* getEventDispatcher();

    /* Returns texture named @filename, or nullptr if not found. */
    static spStormTexture getTexture(const std::string& filename);

    /* Returns currently active scene */
    static SScene* getActiveScene();

    /* Get time in milliseconds (ms), how long was program running */
    static uint32_t getTimeMs();

    /* Get time in nanoseconds (ns), how long was program running */
    static uint64_t getTimeNs();

private:
    static const std::string DEFAULT_SHADER_NAME;

    StormEngine();

    /* Structure that holds information about game window,
     * window size, type, title etc...*/
    StormWindowSettings _WindowInfo;

    /* SEventDispatcher Used for sending notifications between different engine components */
    SEventDispatcher::Dispatcher* _EventDispatcher;

    /***** ENGINE MODULES *****/
    
    /* StormPlatform module, used for handling events, calling main tick method
     * and other platform dependend stuff */
    StormPlatform* _ModPlatform;

    /* StormVideoDriver module, used for initializing glsl,
     * and handling virtual view, and viewport calculations*/
    StormVideoDriver* _ModVideoDriver;

    /* StormRenderer module, used for queuing and executing render commands,
     * and manipulating shader stuff */
    StormRenderer* _ModRenderer;

    /* STextureManager modue, used for handling texture resources. */
    STextureManager* _ModTextureManager;

    /* StormFileSystem module, used for handling game resource files (textures/scenes/etc..) */
    StormFileSystem* _ModResources;

    /* Module used for handling LUA script */
    SLuaSystem* _ModLuaSystem;

    /* SSceneManager module, used for loading/unloading/switching scenes. */
    SSceneManager* _ModSceneManager;

    /* Map of all engine modules mapped by their type id */
    std::map<std::type_index, StormModuleBase*> _ModulesByType;

    /***** END OF ENGINE MODULES *****/

    /* Main ticking function. 
     * Called as callback form platform module */
    void mainTickingMethod(float deltaTime);

    /* Window event listener.
     * Method get's called from platform module when new event occures */
    void windowEventListener(StormWindowEventType event);

    /* Graphics ticking method */
    void renderTick();
    
    /* Logic ticking method */
    void updateTick(float deltaTime);

public:
    /* Returns pointer to engine module of specified type @T
     * Returns nullptr if module is not found.
     * Implemented in header file for compiler compatibility */
    template<class T>
    static T* getModule() { 
        const std::type_index& info = typeid(T);
        auto iter = instance()->_ModulesByType.find(info);
        if (iter == instance()->_ModulesByType.end()) {
            LOG(ERROR) << "Requested engine module not found";
            return nullptr;
        }
        T* tmpPtr = dynamic_cast<T*>(iter->second);
        S_ASSERT(tmpPtr)
        return tmpPtr;
    }
 
};
