Handles = {}

function Vector2:_str()
    return "'X:" .. self.x .. " Y:" .. self.y .. "'"
end

function createObjectHandle(object)
    local handle = {
        obj = object,
        isValid = true
    }
    Handles[object.id] = handle
    return handle
end

function getObjectHandle(id)
    local handle = Handles[id]
    if not handle.isValid then 
        debug.log("Getting invalid entity handle " .. id)
    end
    return handle
end

function tickObjects(deltaTime) 
    for key,value in pairs(Handles) do
        local object = Handles[key]
        if object.isValid and object.obj.onUpdate ~= nil then
            object.obj.onUpdate(deltaTime)
        end
    end
end
