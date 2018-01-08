Handles = {}

-- Meta table used for handle
local HandleMT = {}
HandleMT.__index = function(handle, key)
    -- REMAINDER @handle is self, @key is self.key
    if not handle.isValid then
        -- TODO: Handle error
        debug.log("Invalid handle error 1!")
        debug.log(debug.traceback())
        return nil
    end

    cppRefMember = handle.cppRef[key]
    if cppRefMember ~= nil then
        -- Try to find member in cpp reff usertype object
        if type(cppRefMember) ~= "function" then 
            return cppRefMember 
        else
            return function(...) return cppRefMember(...) end
        end
    elseif handle.hasScript then
        -- Try to find member in script table
        script = handle.script
        scriptMember = script[key]
        if scriptMember ~= nil then
            if type(scriptMember) ~= "function" then 
                return scriptMember 
            else
                return function(...) return scriptMember(...) end
            end
        end
    end

    return nil
end

local ScriptMT = {}
ScriptMT.__index = function(script, key)
    if not script.handle.isValid then
        -- TODO: Handle error
        debug.log("Invalid handle error 2!")
        debug.log(debug.traceback())
        return nil
    end

    cppRefMember = script.cppRef[key]
    if cppRefMember ~= nil then
        -- Try to find member in cpp reff usertype object
        if type(cppRefMember) ~= "function" then 
            return cppRefMember 
        else
            return function(script, ...) return cppRefMember(script.cppRef, ...) end
        end
    end
    return nil
end

function createObjectHandle(cppObjRef)
    local handle = {
        -- Reference to cpp object
        cppRef = cppObjRef,
        
        -- LUA table defined in cpp object's script component
        -- Can be NULL if object dose not have scripts attatched
        script = nil,

        hasScript = false,

        -- Is this reference still valid
        isValid = true
    }

    setmetatable(handle, HandleMT)

    if cppObjRef == nil then
        -- Do not create handle at all if there is no cppReff
        return handle
    end

    Handles[cppObjRef.id] = handle
    return handle
end

-- Called every time new script file has been loaded
-- Parameter @script is lua table returned from script file
-- Parameter @handle is handle of scene object that owns the script
function handleOnScriptLoaded(script, handle)
    handle.script = script
    handle.hasScript = true

    script.handle = handle
    script.cppRef = handle.cppRef
    setmetatable(script, ScriptMT)
end

-- Returns handle for object @id
function getObjectHandle(id)
    local handle = Handles[id]
    if not handle.isValid then 
        debug.log("Getting invalid entity handle " .. id)
    end
    return handle
end

-- Execute this.onUpdate methods for all objects
function tickObjects(deltaTime) 
    for key,value in pairs(Handles) do
        local handle = Handles[key]
        if handle.hasScript then
            handle.onUpdate(deltaTime)
        end
    end
end
