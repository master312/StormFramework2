#include "StormFileSystem.h"
#include "../StormCommon.h"
#include <sys/types.h>
#include <dirent.h>
#include <fstream>

StormFileSystem::StormFileSystem() {
    _RootPath = "";
}

StormFileSystem::StormFileSystem(const std::string& path) {
    _RootPath = path;
}

StormFileSystem::~StormFileSystem() {
    /* All resources are smart pointers, so we dont need delete.
     * Just make sure there are no any references to resources left in other places. */
    _Resources.clear();
}

void StormFileSystem::setRootPath(const std::string& path) {
    _RootPath = path;

    if (_RootPath != "" && _RootPath[_RootPath.size() - 1] != '/') {
        /* Adds '/' character on the end of the path, if there already isn't one. */
        _RootPath.push_back('/');
    }

    LOG(INFO) << "New filesystem created with path " << _RootPath;
}

spStormResourceFile StormFileSystem::getResourceByFilename(const std::string& filename) {
    int filenameHash = StormMiscTools::hashString(filename);

    auto iter = _Resources.find(filenameHash);
    if (iter != _Resources.end()) {
        /* Resource file already loaded */
        return iter->second;
    }
    
    std::string fullPath = _RootPath + filename;
    StormResourceFile* loadedResource = loadResource(fullPath);
    if (!loadedResource) {
        LOG(ERROR) << "Resource '" << fullPath << "' not found.'";
        return nullptr;
    }
    loadedResource->setNumIdentifier(filenameHash);
    _Resources[filenameHash] = loadedResource;
    LOG(DEBUG) << "Resource '" << fullPath << "' loaded";
    return loadedResource;
}

int StormFileSystem::saveResourceFile(spStormResourceFile file, bool keepOpen /* = false */) {
    if (!file->getBufferSize()) {
        LOG(WARNING) << "Tryed to save resource file with buffer size of 0.";
        return -1;
    }
    
    std::string filename = file->getFilenameWithPath();
    std::string fullPath = _RootPath + filename;
    
    std::ofstream out;
    out.open(fullPath, std::ios::out);
    if (!out.is_open()) {
        LOG(ERROR) << "Could not save resource file at path " << fullPath;
        return -2;
    }

    out.write(file->getBuffer(), file->getBufferSize());

    out.close();

    if (keepOpen) {
        int filenameHash = StormMiscTools::hashString(filename);

        auto iter = _Resources.find(filenameHash);
        if (iter != _Resources.end()) {
            /* Resource file already loaded */
            LOG(WARNING) << "Requested to keep file " << filename << " opened, but file is already opened. This might cose strange errors";
        }
        _Resources[filenameHash] = file;
    }

    LOG(DEBUG) << "Resource file saved " << filename;
    return 1;
}

void StormFileSystem::freeResource(spStormResourceFile resource, bool forceUnload /* = false */) {
    int identifier = resource->getNumIdentifier();
    if (!identifier) {
        LOG(ERROR) << "Filesystem tried to free resource '" << resource->getFilenameWithPath() << "' without num identifier. This will probably fail.";
    }

    auto iter = _Resources.find(identifier);
    if (iter == _Resources.end()) {
        LOG(WARNING) << "Filesystem tried to free resource '" << resource->getFilenameWithPath() << "' but resource is not maped";
        return;
    }
    
    /* Checks if there are 3 or less references left.
     * First one is in @_Resources map, second is @resource, 
     * and third is one used in class that called this method.
     * Only then we remove resource from cache. */
    if (resource->_RefCounter <= 3 || forceUnload) {
        _Resources.erase(iter);
    }
}

std::string StormFileSystem::getRootPath() const {
    return _RootPath;
}

std::vector<std::string> StormFileSystem::getFilesList(const std::string& path /* = "" */, 
                                                       const std::string& ext /* = "" */,
                                                       bool fullPath /* = true */) {
    /* TODO: Only temporary solution. Rework this */
    std::vector<std::string> filesList;

    std::string fullDirPath = getRootPath() + path;
    DIR* dirp = opendir(fullDirPath.c_str());
    struct dirent* dp;
    int extSize = ext.size();
    while ((dp = readdir(dirp)) != NULL) {
        if (ext != "") {
            std::string extension = dp->d_name;
            if (extension.size() <= extSize) {
                continue;
            }
            extension = extension.substr(extension.length() - extSize);
            if (extension.compare(ext) != 0) {
                continue;
            }
        }
        if (fullPath) {
            filesList.push_back(path + std::string(dp->d_name));
        } else {
            filesList.push_back(dp->d_name);
        }
    }
    closedir(dirp);
    return filesList;
}

bool StormFileSystem::checkIfFileExists(const std::string& filename) const {
    /* TODO: Fix this shit. .is_open() can fail for many other reasons, 
             other then file not existing. */
    std::ifstream file(_RootPath + filename);
    return file.is_open();
}

StormResourceFile* StormFileSystem::loadResource(const std::string& filename) {
    if (!filename.size()) {
        return nullptr;
    }
    char lastChar = filename.at(filename.size() - 1);
    if (lastChar == '\\' || lastChar == '/') {
        /* Do not open if path is directory */
        return nullptr;
    }

    std::ifstream file;
    size_t size = 0;
    file.open(filename, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        return nullptr;
    }
    
    file.seekg(0, std::ios::end);
    size = file.tellg();
    file.seekg(0, std::ios::beg);

    StormResourceFile* resourceFile = new StormResourceFile(filename);
    
    char* buffer = new char[size + 1];
    file.read(buffer, size);
    /* In case of text file, we add \0 on the end.
     * Binary content should not be affected, sence size var is never changed */
    buffer[size] = '\0';

    resourceFile->setBuffer(buffer, size);

    return resourceFile;
}
