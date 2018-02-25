#pragma once
#include "StormEngine.h"

class StormEngineEditing : public StormEngine {
    friend class StormEngine;

public:
    virtual ~StormEngineEditing();

    static StormEngineEditing* instanceEditing();

    /* Activate tool with @name */
    void activateTool(const std::string& name);

private:
    StormEngineEditing();

};
