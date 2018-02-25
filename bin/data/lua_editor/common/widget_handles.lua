
EditorWidgetHandles = {}
EditorWidgetHandlesCnt = 0

-- Called from engine on new component widget creation
-- Returns table that is used for managing component widget
function createComponentWidgetHandle(cppObjRef, scriptObj)
    EditorWidgetHandlesCnt = EditorWidgetHandlesCnt + 1
    local handle = {
        -- Unique handle index
        index = EditorWidgetHandlesCnt,

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

    EditorWidgetHandles[EditorWidgetHandlesCnt] = handle

    return handle
end

-- Called from engine on component widget destruction
function destroyComponentWidgetHandle(handleIndex)
    local handle = EditorWidgetHandles[handleIndex]
    handle.isValid = false
    handle.cppRef = nil
    EditorWidgetHandles[handleIndex] = nil
end

-- Called from editor_main.lua to tick all component widgets (if any)
function tickComponentWidgets(deltaTime)
    for key,value in pairs(EditorWidgetHandles) do
        local handle = EditorWidgetHandles[key]
        if handle.isValid and handle.hasScript then
            if handle.cppRef.isCollapsed == false then
                -- Tick component ONLY if its not collapsed
                local updateFun = handle.script.onUpdate
                if updateFun ~= nil then
                    updateFun()
                end
            end
        end
    end
end
