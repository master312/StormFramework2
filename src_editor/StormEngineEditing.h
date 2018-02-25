#pragma once
#include "StormEngine.h"

class StormEngineEditing : public StormEngine {
    friend class StormEngine;

public:
    virtual ~StormEngineEditing();

    /* Activate tool with @name */
    void activateTool(const std::string& name);

private:
    StormEngineEditing();

};
