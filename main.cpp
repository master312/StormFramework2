#include <iostream>
#include <vector>
#include "src/core/resources/StormFileSystem.h"
#include "src/core/graphics/StormVideoDriver.h"
#include "src/core/graphics/StormShader.h"
#include "src/core/platforms/StormPlatformSDL2.h"
#include "src/StormEngine.h"
#include <GL/glew.h> 
#include <SDL2/SDL.h>
#include "src/core/graphics/StormRenderer.h"
#include "src/core/graphics/StormTexture.h"
#include "src/StormTextureManager.h"
INITIALIZE_EASYLOGGINGPP

// StormPlatformSDL2* platform = nullptr;
// StormVideoDriver* video = nullptr;
// StormRenderer* renderer = nullptr;

// void initPlatform() {
//     platform = new StormPlatformSDL2();
//     platform->initialize();
//     platform->createWindow(StormWindowSettings(800, 768, false, "TheStormWindow", true));
// }

// void initVideoDriver() {
//     video = new Stor``mVideoDriver();
//     video->initialize(platform->getWindowSettings().getSize());
// }

// void initRenderer() {
//     renderer = new StormRenderer();
//     renderer->initialize();
// }

// uint32_t indexData[] = { 0, 1, 2, 3 };
// StormVertex vertices[4];
// StormTextureManager* textureManager = nullptr;

// void mainTickingFunction(float deltaTime) {
//     if (platform->getInputManager()->isKeyDown(S_KEY_ESCAPE)) {
//         platform->quit();
//     }

//     if (platform->getInputManager()->isPointerDown(S_POINTER_LEFT)) {
//         std::cout << "----- LeftDown -----" << std::endl;
//     }
//     if (platform->getInputManager()->isPointerDown(S_POINTER_RIGHT)) {
//         std::cout << "----- RightDown -----" << std::endl;
//     }
//     if (platform->getInputManager()->isPointerDown(S_POINTER_MIDDLE)) {
//         std::cout << "----- MiddleDown -----" << std::endl;
//     }
    
//     video->begin();
//     video->clear();

//     StormRenderCommand* cmd = renderer->newCommand();
//     cmd->setTexture(textureManager->getTexture("icon.png").get());
//     cmd->setVertexData(vertices, 4);
//     cmd->setIndexData(indexData, 4);

//     renderer->queueCommand(cmd);
//     renderer->flush();

//     /*      
//     StormRenderCommand();
//     ~StormRenderCommand();

//     void setTexture(StormTexture* texture);
//     void setVertexData(StormVertex* vertices, uint16_t count);
//     void setIndexData(uint32_t* indices, uint16_t count);

//     */

//     platform->windowSwapBuffers();
//     //SDL_Delay(1);
// }

int main(int argc, char* argv[]) {
    el::Configurations c;
    c.setToDefault();
    c.parseFromText("*GLOBAL:\n FORMAT = %level %msg");


    StormEngine::instance()->initialize(STORM_PLATFORM_SDL2);
    StormEngine::instance()->startMainLoop();
    StormEngine::instance()->deinitialize();
    
    return 1;

    // StormFileSystem* fs = new StormFileSystem();
    // fs->setRootPath("data/");

    // textureManager = new StormTextureManager(fs);

    // initPlatform();
    // initVideoDriver();
    // initRenderer();
    
    // StormShader* shader = new StormShader();
    // shader->compileFromSource(fs->getResourceByFilename("color.vs")->getBuffer(), 
    //                                            fs->getResourceByFilename("color.fs")->getBuffer());
    // shader->linkShaders();
    // shader->use();

    // renderer->setViewMatrix(0.0f, 0.0f, 800.0f * 1.3, 768.0f * 1.3);
    // platform->getInputManager()->calculatePointerScaling(Vector2(800*1.3f, 768*1.3f));
    
    // renderer->setShader(shader);

    
    // vertices[0].position.set(-0.5, -0.5); // BOT LEFT
    // vertices[0].uv.set(0.0f, 0.0f);

    // vertices[1].position.set(0.5, -0.5); // BOTTOM RIGHT
    // vertices[1].uv.set(1.0f, 0.0f);

    // vertices[2].position.set(0.5, 0.5); // TOP RIGHT
    // vertices[2].uv.set(1.0f, 1.0f);

    // vertices[3].position.set(-0.5, 0.5); // TOP LEFT
    // vertices[3].uv.set(0.0f, 1.0f);

    // for (int i = 0; i < 4;i ++) {
    //     vertices[i].position *= 100;
    //     vertices[i].position.x += 800;
    //     vertices[i].position.y += 100;
    // }

    // platform->setMainTickingFunction(&mainTickingFunction);
    // platform->startMainLoop();

    return 0;
}