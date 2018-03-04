#pragma once
#include "StormEngine.h"

class StormEngineEditing : public StormEngine {
    friend class StormEngine;

public:
    virtual ~StormEngineEditing();

    static StormEngineEditing* instanceEditing();

    /* Saves currently active scene to file.
     * If @path is not specified, scene will be saved to default location. */
    void saveActiveScene(const std::string& path = "");

    /* Activate tool with @name */
    void activateTool(const std::string& name);

private:
    StormEngineEditing();

};
