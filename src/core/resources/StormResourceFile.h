#pragma once
#include "../StormCommon.h"

/* This class is just container for resource file data. 
 * All loading/saving stuff are in StormFileSystem class.
 * File path is relative to filesystem path. */

class StormResourceFile : public StormRefCounter {
public:
    StormResourceFile();
    StormResourceFile(const std::string& filename);
    virtual ~StormResourceFile();

    /* Creates new buffer of @size */
    void allocateBuffer(uint32_t size);
    
    /* Creates new buffer of @size and coppy data from @srcBuffer into new one.
     * @srcBuffer will be unchanged, and it will not be deleted by StormResourceFile.  */
    void allocateBuffer(uint32_t size, char* srcBuffer);

    /* Sets pointer to existing buffer. Ownership of the buffer will be taken
     * by StormResourceFile object, and it will be freed when file gets deleted */
    void setBuffer(char* buffer, uint32_t size);
    
    /* Returns pointer to file data buffer */
    char* getBuffer();
    
    /* Returns file buffer size in bytes */
    uint32_t getBufferSize();
    
    /* Deletes buffer */
    void freeBuffer();
    
    /* Sets filename to resource file.
     * @filename should be in format "path/to/file/filename.txt" 
     * When set, filename will automatically be splitted into path, filename, and extension. */
    void setFilename(const std::string& filename);
    
    void setNumIdentifier(int identifier);
    int getNumIdentifier();

    /* Returns filename without path and extension */
    std::string getFilename() const;
    /* Returns extension without dot */
    std::string getExtension() const;
    /* Returns filename.extension */
    std::string getFilenameWithExt() const;
    /* Returns full path of the file. */
    std::string getPath() const;
    /* Returns path/filename.extension format */
    std::string getFilenameWithPath() const;
    /* Returns file size in kilobytes */
    uint32_t getFileSizeInKb() const;

    /* Gets string containting file info */
    std::string toString() const;
private:
    /* Name of actual file, without path and extension */
    std::string _Filename;
    /* Extension, without dot(.) (e.x. png, exe, bin...) */
    std::string _Extension;
    /* File path */
    std::string _Path;
    
    /* Buffer of data loaded from file */
    char* _DataBuffer;
    /* Size of data buffer */
    uint32_t _BufferSize;

    /* Custom resource identifier. 
     * This will most likely be hashed resource filename. */
    int _Identifier;
};


typedef StormIntrusivePtr<StormResourceFile> spStormResourceFile;