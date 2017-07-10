#pragma once
#include "StormResourceFile.h"
#include "../utils/StormMiscTools.h"
#include <map>

/* Filesystem class that handles loading/saving/unloading/deletion of resource files. 
 * INFO: All paths written in format: "c:\test\game\" will be converted to "c:/test/game/'.
 * All loaded resource object are unloaded in destructor, but since all resource files are 
 * smart pointers, make sure there aren't any references left to resource, or it wont be unloaded. */

/* TODO
 ---- Long term ----
 - Upon setting filesystem path using @setRootPath(...) check if path exists, and report errors if needed
 - Disable loading (only already loaded resources can be getted)
 - Some kind of XML file that indexes all resource by identifiers for easyer acces 
*/

class StormFileSystem {
public:
    StormFileSystem();
    StormFileSystem(const std::string& path);
    ~StormFileSystem();

    /* Sets folder that will be used as root of this filesystem. 
     * @path is full path to folder. */
    void setRootPath(const std::string& path);

    /* Returns smart pointer to resource, or nullptr if not found.
     * If resource is not loaded, this method will load it, so it might take a long time to complete.
     * @filename is path and filename (E.X data/something/image.png) */
    spStormResourceFile getResourceByFilename(const std::string& filename);

    /* Saves @file to filesystem. Filesystem root path will be added to filename.
     * If @keepOpen is set, resource file will be stored in memory
     * and used by @getResourceByFilename method. If file with same name is
     * already opened, it will be overriden by new file.
     * Returns < 0 on error. */
    int saveResourceFile(spStormResourceFile file, bool keepOpen = false);

    /* Tell filesystem class that we are done with resource, 
     *  and it can be deleted if not used by anything else.
     * If @forceUnload is set to true, resource will be deleted from @_Resources map 
     *  without any safety checks first. 
     * WARNING: Use @forceUnload only if you know what are you doing. This can cause memory leaks. */
    void freeResource(spStormResourceFile resource, bool forceUnload = false);

    /* Returns string containing path to root of this filesystem */
    std::string getRootPath() const;

private:
    /* Root path of filesystem. (E.X: c:/game/data/ - with'/' on the end)*/
    std::string _RootPath;
    /* Map of all currently loaded resource files, indexed by their hashed filename. */
    std::map<int, spStormResourceFile> _Resources;

    StormResourceFile* loadResource(const std::string& filename);
};
