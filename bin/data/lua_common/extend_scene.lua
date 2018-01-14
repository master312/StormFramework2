
function Scene:instantiatePrefab(prefabName, newObjectName)
    cppObject = self:intInstantiatePrefab(prefabName, newObjectName)
    return getObjectHandle(cppObject.id)
end

function Scene:destroyObject(handle)
    self:intDestroyObject(handle.id)
end
