#pragma once
#include "core/platforms/StormPlatform.h"
#include "StormTextureManager.h"

/* Singleton class responsible for initialization and deinitialization of all engine components */

/*
    * TODO:
        - Load configuration from file
*/

class StormVideoDriver;
class StormRenderer;
class StormShader;

class StormEngine {
public:
    virtual ~StormEngine();

    /* Gets singleton instance */
    static StormEngine* instance();

    /* Initialize all engine components. 
     * If there was an error during initialization, if will be logged, and program will crash */
    void initialize(StormPlatformType platform);

    /* TODO: Maybe not needed here? Deinitialize all components, frees all memory, and ends main loop */
    void deinitialize();

    /* Starts engine main loop. This method will block until program ends */
    void startMainLoop();

    /* Stops engine */
    void quit();

    /* Returns pointer to renderer */
    StormRenderer* getRenderer();

    /* Returns pointer to platform subsystem */
    StormPlatform* getPlatform();

    /* Returns pointer to texture manager */
    StormTextureManager* getTextureManager();

    /* Returns pointer to filesystem containing all game data */
    StormFileSystem* getDataFilesystem();
    
private:
    StormEngine();

    bool _IsInitialized;

    /** Engine settings **/
    /* Informations about game window */
    StormWindowSettings _WindowInfo;
    
    /* Currently initialized platform subsystem */
    StormPlatform* _Platform;

    /* Engine components */
    StormVideoDriver* _ComVideoDriver;
    StormRenderer* _ComRenderer;
    StormTextureManager* _ComTextureManager;

    /* Filesystem containing all game data (textures/scenes/etc..) */
    StormFileSystem* _GameDataFilesystem;

    /* Default shader that will be use if none is set */
    StormShader* _DefaultShader;

    /* Initialize all engine components (video/audio/etc...)
     * Returns < 0 on failure */
    int initializeComponents();

    /* Deinitialize all components and free memory */
    void deinitializeComponents();

    /* Main ticking function. Called form platform subsystem */
    void mainTickingMethod(float deltaTime);

    /* This method gets called on window events. (like resize, hide, move and stuff) */
    void windowEventListener(StormWindowEventType event);

    /* Graphics ticking method */
    void renderTick();
    
    /* Logic ticking method */
    void updateTick(float deltaTime);
};