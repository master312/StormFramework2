 
#include "src/StormCommon.h"
#include <iostream>
#include <vector>
#include "src/resources/StormFileSystem.h"
#include "src/display/StormWindow.h"
#include "src/graphics/StormVideoDriver.h"
#include "src/graphics/StormShader.h"
#include <GL/glew.h> 
INITIALIZE_EASYLOGGINGPP

#include "src/utils/math/Rect.h"

int main(int argc, char* argv[]) {

    StormFileSystem* fs = new StormFileSystem();
    fs->setRootPath("data/");

    spStormResourceFile resFile = fs->getResourceByFilename("tstFile.txt");

    Rect E;
    Vector2 vec;
    vec.x = 1;
    for (int i = 0; i < 5; i++) {
        std::cout << fs->getResourceByFilename("tstFile.txt")->toString() << std::endl;
    }

    StormWindow::instance()->create(1280, 768, false);
    StormVideoDriver::instance()->initialize();

    StormShader* shader = new StormShader();
    shader->compileFromSource(fs->getResourceByFilename("color.vs")->getBuffer(), 
                                               fs->getResourceByFilename("color.fs")->getBuffer());
    shader->addAttribute("vertexPosition");
    shader->linkShaders();
                                               

    uint32_t vboId = 0;
    glGenBuffers(1, &vboId);
    if (vboId == 0) {
        LOG(FATAL) << "HEREEEEEEEE";
    }

    float x = -1;
    float y = -1;
    float w = 1;
    float h = 1;

    float vertexData[12];
    vertexData[0] = x + w;
    vertexData[1] = y + h;

    vertexData[2] = x;
    vertexData[3] = y + h;

    vertexData[4] = x;
    vertexData[5] = y;

    vertexData[6] = x;
    vertexData[7] = y;

    vertexData[8] = x + w;
    vertexData[9] = y;

    vertexData[10] = x + w;
    vertexData[11] = y + h;

    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    uint32_t i = 0;
    while(i < 150) {
        StormVideoDriver::instance()->clear();
        StormVideoDriver::instance()->begin();

        shader->use();
        glBindBuffer(GL_ARRAY_BUFFER, vboId);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        float tmpTime = (float)(SDL_GetTicks()) / 100.0f; 
        uint32_t loc = shader->getUniformLocation("time");
        glUniform1f(loc, tmpTime);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
       shader->unuse();

        
        StormWindow::instance()->swapBuffers();
        i++;
        SDL_Delay(1);
    }

    glDeleteBuffers(1, &vboId);
    StormWindow::instance()->destroy();

    //SDL_Delay(2000);

    /*StormResourceFile* fl = new StormResourceFile();
    fl->setFilename("shitload/of/adata/save.dat");
    
    std::cout << fl->getFilename() << std::endl;
    std::cout << fl->getExtension() << std::endl;
    std::cout << fl->getPath() << std::endl;

    LOG(INFO) << fl->toString();*/
    

    return 0;
}