Handles = {}

-- This :_str method is unfunctional...
function Vector2:_str()
    return "'X:" .. self.x .. " Y:" .. self.y .. "'"
end

function createObjectHandle(cppRef)
    local handle = {
        -- Reference to cpp object
        cppRef = cppObject,
        
        -- LUA table defined in cpp object's script component
        -- Can be NULL if object dose not have scripts attatched
        script = nil,

        -- Is this reference still valid
        isValid = true
    }

    if cppRef == nil then
        -- Do not create handle at all if there is nto cppReff
        return handle
    end

    Handles[cppRef.id] = handle
    return handle
end

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
        if handle.script ~= nil then
            if handle.isValid and handle.script.onUpdate ~= nil then
                handle.script.onUpdate(deltaTime)
            end
        end
    end
end
