 
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

    scriptObj.handle = handle
    scriptObj.cppRef = handle.cppRef
    -- .cppReff and .widget are the same things
    -- .widget is only created for cleaner code
    scriptObj.widget = handle.cppRef
    setmetatable(scriptObj, ScriptMT)

    return handle
end


debug.log("...........6....6..................")
debug.log("............6..6...................")
debug.log(".............66....................")
debug.log("ETITORRRR common file loaded ma man")
debug.log(".............66....................")
debug.log("............6..6...................")
debug.log("...........6....6..................")

-- local parentShitSomething = debug.getQTWidget()

-- local prop = EPropVec2.new(parentShitSomething)
-- prop:setValue(Vector2.new(100,100))

-- for i=1,10 do 
--     print(i) 
--    -- EPropVec2.new(parentShitSomething)
-- end