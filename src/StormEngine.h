#pragma once
#include <typeindex>
#include "core/platforms/StormPlatform.h"
#include "core/resources/StormFileSystem.h"
#include "StormTextureManager.h"

/* Singleton class responsible for initialization and deinitialization of all engine modules */

class StormVideoDriver;
class StormRenderer;
class SSceneManager;
class StormScene;

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

    /**** Commonly used module methods for easy access ****/
    /* Returns resource file from default filesystem,
     * or nullptr if not found */
    static spStormResourceFile getResource(const std::string& filename);    

    /* Returns currently active scene */
    static StormScene* getActiveScene();

private:
    static const std::string DEFAULT_SHADER_NAME;

    StormEngine();

    /* Struct that holds informations about game window,
     * window size, type, title etc...*/
    StormWindowSettings _WindowInfo;
    
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

    /* StormTextureManager modue, used for handling texture resources. */
    StormTextureManager* _ModTextureManager;

    /* StormFileSystem module, used for handling game resource files (textures/scenes/etc..) */
    StormFileSystem* _ModResources;

    /* SSceneManager module, used for loading/unloading/switching scenes. */
    SSceneManager* _ModSceneManager;

    /* Map of all elgine modules maped by their type id */
    std::map<std::type_index, void*> _ModulesByType;

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
        T* tmpPtr = static_cast<T*>(iter->second);
        if (!tmpPtr) {
            LOG(ERROR) << "StormEngine::getModule casting error";
            return nullptr;
        }
        return tmpPtr;
    }
 
};
