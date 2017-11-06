#include "StormTextureManager.h"
#include "core/StormCommon.h"
#include "StormEngine.h"
#include "core/resources/StormFileSystem.h"

StormTextureManager::StormTextureManager() {
    _Filesystem = StormEngine::getModule<StormFileSystem>();
    
    /* Load default texture */
    _DefaultTexture = getTexture(DEFAULT_TEXTURE);
    if (_DefaultTexture) {
        _DefaultTexture->setIsDefaultTexture(true);
    } else {
        LOG(WARNING) << "Default texture not found! This might crash...";
    }
}

StormTextureManager::~StormTextureManager() {
    /* Since all textures are smart pointers, there is not need to delete */
}

spStormTexture StormTextureManager::getTexture(const std::string& filename) {
    if (filename == "") {
        LOG(ERROR) << "Tryed to get texture, but texture name was empty";
        return nullptr;
    }
    int filenameHash = StormMiscTools::hashString(filename);
    
    auto iter = _LoadedTextures.find(filenameHash);
    if (iter != _LoadedTextures.end()) {
        /* Texture is already loaded in memory */
        return iter->second;
    }

    spStormTexture tmpTexture = loadTextureResource(filename);
    if (!tmpTexture) {
        /* Returns default texture if requested is not found */
        LOG(ERROR) << "Could not load texture file";
        return _DefaultTexture;
    }
    
    _LoadedTextures[filenameHash] = tmpTexture;
    return tmpTexture;
}

void StormTextureManager::freeAllUnusedTextures() {
    for (auto iter : _LoadedTextures) {
        if (iter.second->_RefCounter <= 1) {
            /* Texture is smart pointer. 
             * Memory will be freed reference from @_LoadedTextures is lost */
            _LoadedTextures.erase(iter.first);
        }
    }
}

spStormTexture StormTextureManager::loadTextureResource(const std::string& filename) {
    if (!_Filesystem) {
        LOG(ERROR) << "Could not load texture resource. No filesystem specified";
        return nullptr;
    }
    
    spStormResourceFile file = _Filesystem->getResourceByFilename(filename);
    if (!file) {
        return nullptr;
    }
    
    spStormTexture tmpTexture = new StormTexture();
    if (tmpTexture->loadFromFile(file) < 0) {
        LOG(ERROR) << "Error loading texture from file " << filename;
        _Filesystem->freeResource(file);
        return nullptr;
    }
    
    tmpTexture->setName(filename);

    /* Close resource file */
    _Filesystem->freeResource(file);
    LOG(DEBUG) << "Texture file '" << filename << "' loaded";
    return tmpTexture;
}