#include "StormResourceFile.h"
#include <algorithm>
#include <sstream>
#include <memory.h>

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

    size_t filenameStart = replacedFilename.find_last_of("\\/");
    if (filenameStart != std::string::npos) {
        /* Path exists */
        _Filename = replacedFilename.substr(filenameStart + 1);
        _Path = replacedFilename.substr(0, filenameStart);
    } else {
        /* Path dose not exists. Only filename */
        _Filename = replacedFilename;
        _Path = "";
        LOG(WARNING) << "No path found in resource file path '" << _Filename << "'";
    }

    size_t extDotPosition = _Filename.find_last_of(".");
    if (extDotPosition == std::string::npos || extDotPosition >= _Filename.size() - 1) {
        /* Extension not specified */
        _Extension = "";
        LOG(WARNING) << "No extension found in resource file path '" << filename << "'";
    } else {
        /* Extension exists */
        _Extension = _Filename.substr(extDotPosition + 1);
        _Filename = _Filename.substr(0, extDotPosition);
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
