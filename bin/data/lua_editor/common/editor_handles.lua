 IsEditor = true
 EditorHandles = {}
 EditorHandlesCnt = 0

-- Called from c++ on new component widget creation
-- Returns table that is used for managing component widget
function createComponentWidgetHandle(cppObjRef, scriptObj)
    EditorHandlesCnt = EditorHandlesCnt + 1
    local handle = {
        -- Unique handle index
        index = EditorHandlesCnt,

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

    EditorHandles[EditorHandlesCnt] = handle

    return handle
end

function destroyComponentWidgetHandle(handleIndex)
    local handle = EditorHandles[handleIndex]
    handle.isValid = false
    handle.cppRef = nil
    EditorHandles[handleIndex] = nil
end

EditorUpdateDeltaTime = 0
function editorTickObjects(deltaTime)
    -- Tick editor scripts
    EditorUpdateDeltaTime = EditorUpdateDeltaTime + deltaTime
    if EditorUpdateDeltaTime >= 0.25 then
        -- Editor scripts are updated 4 times per second
        EditorUpdateDeltaTime = 0
        for key,value in pairs(EditorHandles) do
            local handle = EditorHandles[key]
            if handle.isValid and handle.hasScript then
                local updateFun = handle.script.onUpdate
                if updateFun ~= nil then
                    updateFun()
                end
            end
        end
    end
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