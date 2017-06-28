#include "StormResourceFile.h"
#include <algorithm>

StormResourceFile::StormResourceFile() {
    _Filename = "";
    _Extension = "";
    _Path = "";

    _DataBuffer = nullptr;
    _BufferSize = 0;
    _Identifier = 0;
}

StormResourceFile::StormResourceFile(const std::string& filename) {
    StormResourceFile();
    setFilename(filename);
}

StormResourceFile::~StormResourceFile() {
    freeBuffer();
}

void StormResourceFile::allocateBuffer(uint32_t size) {
    _DataBuffer = new char[size];
    _BufferSize = size;
}

void StormResourceFile::allocateBuffer(uint32_t size, char* srcBuffer) {
    allocateBuffer(size);
    memcpy(_DataBuffer, srcBuffer, size * sizeof(char));
}

void StormResourceFile::setBuffer(char* buffer, uint32_t size) {
    _DataBuffer = buffer;
    _BufferSize = size;
}

char* StormResourceFile::getBuffer() {
    return _DataBuffer;
}

uint32_t StormResourceFile::getBufferSize() {
    return _BufferSize;
}

void StormResourceFile::freeBuffer() {
    if (_DataBuffer) {
        delete _DataBuffer;
    }
    _DataBuffer = nullptr;
    _BufferSize = 0;
    _Identifier = 0;
    LOG(DEBUG) << "Resource '" << getFilenameWithPath() << "' unloaded";
}

void StormResourceFile::setFilename(const std::string& filename) {
    /* Convert all '\' to '/', and split filename into path, extension and filename. */
    std::string replacedFilename = filename;
    std::replace(replacedFilename.begin(), replacedFilename.end(), '\\', '/');
    
    size_t sep = replacedFilename.find_last_of("\\/");
    size_t dot = replacedFilename.find_last_of(".");
    if (sep != std::string::npos && dot != std::string::npos) {
        _Filename = replacedFilename.substr(sep + 1, replacedFilename.size() - dot + sep - 1);
    }
    
    if (dot != std::string::npos) {
        _Extension = replacedFilename.substr(dot + 1, replacedFilename.size() - dot);
        if (sep == std::string::npos) {
            /* No path found. Filename will be set from begining of the string, to the dot. */
            _Filename = replacedFilename.substr(0, replacedFilename.size() - dot);
        }
    } else {
        LOG(WARNING) << "No extension found in resource file path '" << filename << "'";
    }

    if (sep != std::string::npos) {
        _Path = replacedFilename.substr(0, sep);
        if (dot == std::string::npos) {
            /* If dot is not found, filename will be set from last / to the end of string */
            _Filename = replacedFilename.substr(sep + 1, replacedFilename.size());
        }
    } else {
        LOG(WARNING) << "No path found in resource file path '" << filename << "'";
    }
    
    if (sep == std::string::npos && dot == std::string::npos) {
        /* No extension and path found */
        _Filename = filename;
    }
}

void StormResourceFile::setNumIdentifier(int identifier) {
    _Identifier = identifier;
}

int StormResourceFile::getNumIdentifier() {
    return _Identifier;
}

std::string StormResourceFile::getFilename() const {
    return _Filename;
}

std::string StormResourceFile::getExtension() const {
    return _Extension;
}

std::string StormResourceFile::getFilenameWithExt() const {
    if (!_Extension.size()) {
        return _Filename;
    }
    return _Filename + "." + _Extension;
}

std::string StormResourceFile::getPath() const {
    return _Path;
}

std::string StormResourceFile::getFilenameWithPath() const {
    return getPath() + "/" + getFilenameWithExt();
}

uint32_t StormResourceFile::getFileSizeInKb() const {
    return (uint32_t)_BufferSize / 1024;
}

std::string StormResourceFile::toString() const {
    std::stringstream ss;
    ss << "StormResourceFile ";
    ss << getFilenameWithPath();
    ss << " size(kb) " << getFileSizeInKb();
    return ss.str().c_str();
}