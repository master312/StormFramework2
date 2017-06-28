#pragma once
#include <SDL2/SDL.h>
#include "../StormCommon.h"

class StormTexture : public StormRefCounter {
public:
    StormTexture();
    ~StormTexture();

    /* Loads texture from file data buffer. Supports SDL2_Image formats. 
     * Returns < 0 on error. */
    int loadFromFileBuffer(const char* buffer, int size);

    /* Created OpenGL texture form pixels buffer */
    int loadFromBuffer(unsigned char* buffer, int width, int height, int bpp);

    uint32_t getOpenGLTextureId();

    void setName(const std::string& name);
    std::string getName() const;

    void setIsDefaultTexture(bool isDefault);
    bool isDefaultTexture() const;
private:
    uint32_t _GLTextureId;

    /* Texture name. Used for texture identification */
    std::string _Name;

    /* Set to true, if texture is a default texture
     * used when texture some is missing, and not actual game asset */
    bool _IsDefaultTexture;

    uint32_t sdlSurfaceToGLTexture(SDL_Surface* surface);
};

typedef StormIntrusivePtr<StormTexture> spStormTexture;