#pragma once

/* Base class for all storm core modules. */

class StormModuleBase {
public:
    StormModuleBase(const std::string& modName) : moduleName(modName) { }
    virtual ~StormModuleBase() { }

    const std::string getModuleName() { return moduleName; }

    StormModuleBase* getModuleBase() { return (StormModuleBase*)this; }

private:
    const std::string moduleName;
};