#include "StormTexture.h"
#include <SDL2/SDL_image.h>
#include <GL/glew.h>
#include <GL/glu.h>

StormTexture::StormTexture() {
    _GLTextureId = 0;
    _IsDefaultTexture = false;
    _Name = "";
}

StormTexture::~StormTexture() {
    if (_GLTextureId) {
        glDeleteTextures(1, &_GLTextureId);
    }
}

int StormTexture::loadFromFileBuffer(const char* buffer, int size) {
    SDL_Surface* texture = IMG_Load_RW(SDL_RWFromMem((void*)buffer, size), 0);
    if (!texture) {
        LOG(ERROR) << "StormTexture Could not load texture from buffer";
        return -1;
    }
    _GLTextureId = sdlSurfaceToGLTexture(texture);
    SDL_FreeSurface(texture);
    if (!_GLTextureId) {
        LOG(ERROR) << "StormTexture Could not convert SDL surface to OpenGL texture";
        return -1;
    }
    LOG(DEBUG) << "GL texture created!";
    return 1;
}

int StormTexture::loadFromBuffer(unsigned char* buffer, int width, int height, int bpp) {
    glGenTextures(1, &_GLTextureId);
    glBindTexture(GL_TEXTURE_2D, _GLTextureId);

    int32_t mode = GL_RGB;
    if(bpp == 4) {
        mode = GL_RGBA;
    } else if (bpp == 1) {
        mode = GL_LUMINANCE;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (_GLTextureId > 0) {
        LOG(DEBUG) << "GL texture generated from pixel buffer";
    } else {
        LOG(ERROR) << "Could not generate GL texture from pixel buffer";
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

uint32_t StormTexture::getOpenGLTextureId() {
    return _GLTextureId;
}

void StormTexture::setName(const std::string& name) {
    _Name = name;
}

std::string StormTexture::getName() const {
    return _Name;
}

void StormTexture::setIsDefaultTexture(bool isDefault) {
    _IsDefaultTexture = isDefault;
}

bool StormTexture::isDefaultTexture() const {
    return _IsDefaultTexture;
}

uint32_t StormTexture::sdlSurfaceToGLTexture(SDL_Surface* surface) {
    uint32_t textureId = 0;
    /* Create The Texture */
    glGenTextures(1, &textureId);
    /* Typical Texture Generation Using Data From The Bitmap */
    glBindTexture(GL_TEXTURE_2D, textureId);

    uint32_t mode = GL_RGB;
    if(surface->format->BytesPerPixel == 4) {
        mode = GL_RGBA;
    }

    /* Generate The Texture */
    glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode,
                          GL_UNSIGNED_BYTE, surface->pixels);
    glGetError();

    /* Linear Filtering 
        qualities are (in order from worst to best)
        GL_NEAREST
        GL_LINEAR
        GL_LINEAR_MIPMAP_NEAREST
        GL_LINEAR_MIPMAP_LINEAR 
    */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureId;
}