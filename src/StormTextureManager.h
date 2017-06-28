#pragma once
#include <iostream>
#include "core/graphics/StormTexture.h"

/*
    * TODO:
        - Return default texture if requested one is not found.
        - Texture preloading system (textures are now loaded in @getTexture() method)
*/

class StormFileSystem;

class StormTextureManager {
public:
    StormTextureManager(StormFileSystem* filesystem);
    ~StormTextureManager();

    spStormTexture getTexture(const std::string& filename);

    /* Unloads all texture resources that are currently not in use */
    void freeAllUnusedTextures();

private:
    /* Default texture that will be used if requested one is not found */
    const std::string DEFAULT_TEXTURE = "default.png";

    /* Resources manager used by texture manager */
    StormFileSystem* _Filesystem;

    /* All currently loaded textures. Maped by texture filename hash. */
    std::map<int, spStormTexture> _LoadedTextures;

    /* This texture will be returned if requested texture is not found */
    spStormTexture _DefaultTexture;

    spStormTexture loadTextureResource(const std::string& filename);
};
