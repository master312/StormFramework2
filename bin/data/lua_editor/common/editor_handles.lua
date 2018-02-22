 
-- Called from c++ on new component widget creation
-- Returns table that is used for managing component widget
function createComponentWidgetHandle(cppObjRef, scriptObj)
    local handle = {
        -- Reference to cpp object
        cppRef = cppObjRef,
        
        -- LUA table defined in object's script component
        script = scriptObj,

        hasScript = true,

        -- Is this reference still valid
        isValid = true
    }
    setmetatable(handle, HandleMT)

    script.handle = handle
    script.cppRef = handle.cppRef
    setmetatable(script, ScriptMT)

    return handle
end


debug.log("ETITORRRR common file loaded ma man")