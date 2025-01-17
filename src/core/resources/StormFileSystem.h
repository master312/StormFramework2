#pragma once
#include <map>
#include "StormResourceFile.h"
#include "../utils/StormMiscTools.h"
#include "../StormModuleBase.h"

/* Filesystem class that handles loading/saving/unloading/deletion of resource files. 
 * INFO: All paths written in format: "c:\test\game\" will be converted to "c:/test/game/'.
 * All loaded resource object are unloaded in destructor, but since all resource files are 
 * smart pointers, make sure there aren't any references left to resource, or it wont be unloaded. */

/* TODO
 ---- Long term ----
 ---** Reimplement @StormFileSystem::checkIfFileExists in safe and portable way! **---
 ---** Maybe if smart pointer ref counter reaches zero (or 3, wathever) automatically free resource buffer **---
 - Upon setting filesystem path using @setRootPath(...) check if path exists, and report errors if needed
 - Disable loading (only already loaded resources can be getted)
 - Some kind of XML file that indexes all resource by identifiers for easyer acces 
*/

class StormFileSystem : public StormModuleBase {
public:
    StormFileSystem();
    StormFileSystem(const std::string& path);
    virtual ~StormFileSystem();

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

    /* Returns string containing path to root of this filesystem
     * with '/' char at the end */
    std::string getRootPath() const;

    /* Returns vector of files in directory @path.
     * If @ext parameter is specified, only filenames of specified extension will be returned.
     * Extension should be specified without dot.
     * If @fullPath is set to false, only filenames will be returned.
     * If @fullPath is set to true, that path will be relative to filesystem root path.
     * If path is empty, root filesystem path will be used */ 
    std::vector<std::string> getFilesList(const std::string& path = "", 
                                          const std::string& ext = "", bool fullPath = true);

    /* Checks if file @filename exists on the filesystem. */
    bool checkIfFileExists(const std::string& filename) const;

    /* Reloads all cached (currently loaded) resources */
    void reloadResources();

private:
    /* Root path of filesystem. (E.X: c:/game/data/ - with'/' on the end)*/
    std::string _RootPath;

    /* Map of all currently loaded resource files, indexed by their hashed filename. */
    std::map<int, spStormResourceFile> _Resources;

    StormResourceFile* loadResource(const std::string& filename);
};
