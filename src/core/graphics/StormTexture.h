#pragma once
#include <SDL2/SDL.h>
#include "../StormCommon.h"
#include "../utils/math/Vector2.h"
#include "../utils/math/Rect.h"

class StormResourceFile;
typedef StormIntrusivePtr<StormResourceFile> spStormResourceFile;

class StormTexture : public StormRefCounter {
public:
    StormTexture();
    ~StormTexture();

    /* Loads texture from file. 
     * Supports all formats that SDL2_Image lib supports. 
     * Returns < 0 on error. */
    int loadFromFile(spStormResourceFile file);

    /* Created OpenGL texture form pixels buffer */
    int loadFromBuffer(unsigned char* buffer, int width, int height, int bpp);

    uint32_t getOpenGLTextureId();

    void setName(const std::string& name);
    std::string getName() const;

    void setIsDefaultTexture(bool isDefault);
    bool isDefaultTexture() const;
    
    Point getSize() const;

    /* Returns area representing usable texture space.
     * This is set to size of whole texture, 
     * or texture part size and offset, in case of atlas */
    std::reference_wrapper<const Rect> getArea() const;

private:
    uint32_t _GLTextureId;

    /* Texture name. Used for texture identification */
    std::string _Name;

    /* Set to true, if texture is a default texture
     * used when texture some is missing, and not actual game asset */
    bool _IsDefaultTexture;

    /* Texture size in pixels */
    Point _Size;

    /* Texture render rectange in pixels.
     * This is set to size of whole texture, 
     * or texture part size and offset, in case of atlas. */
    Rect _Rect;

    uint32_t sdlSurfaceToGLTexture(SDL_Surface* surface);
};

typedef StormIntrusivePtr<StormTexture> spStormTexture;