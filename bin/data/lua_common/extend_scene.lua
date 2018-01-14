
function Scene:instantiatePrefab(prefabName, newObjectName)
    cppObject = self:intInstantiatePrefab(prefabName, newObjectName)
    return getObjectHandle(cppObject.id)
end

function Scene:destroyObject(handleOrId)
    if type(handleOrId) == "number" then
        self:intDestroyObject(handleOrId)
    elseif handleOrId.id ~= nil then
        self:intDestroyObject(handleOrId.id)
    else
        debug.log("Scene:destroyObject invalid argument");
    end
end
