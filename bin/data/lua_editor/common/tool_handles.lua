-- Table (array) to keep all tool tables
EditorToolHandles = {}
-- Name of currently active tool
EditorActiveToolName = nil
-- Table of currently active tool
EditorActiveTool = nil

-- Called from engine every time new tool script gets loaded
-- Should return whether tool is successfully initialized or not
function createToolHandle(scriptObj)
	if scriptObj.name == nil then
		debug.log("ERROR: Tool name not specified.")
		return false
	end
	if scriptObj.label == nil then
		debug.log("ERROR: Tool label not specified.")
		return false
	end

	EditorToolHandles[scriptObj.name] = scriptObj

	if scriptObj.onCreated == nil then
		debug.log("WARNING: Tool script dose not contain .onCrated function")
	else
		scriptObj.onCreated()
	end

	return true
end


-- Called from engine when tool has been selected from GUI
function selectTool(name)
	if EditorActiveToolName == name then
		-- Tried to activate same tool twice
		return
	end

	if EditorActiveToolName ~= nil then
		-- Some tool was already activated
		-- Deactivate it first
		local currentTool = EditorToolHandles[EditorActiveToolName]
		if currentTool == nil then
			debug.log("ERROR: Currently active tool dose not exist in tool handles array. " .. EditorActiveToolName)
		else
			currentTool.onDeactivated()
		end
	end
	EditorActiveToolName = name
	EditorActiveTool = EditorToolHandles[EditorActiveToolName]

	if EditorActiveTool == nil then
		debug.log("ERROR: Tool script for tool '" .. name .. "' dose not exist!")
		return
	end

	EditorActiveTool.onActivated()
end


-- Called every time new scene object has been selected in hierarchy list
function toolHandleObjectSelected(object)
	if EditorActiveTool ~= nil then
		if EditorActiveTool.onObjectSelected ~= nil then
			EditorActiveTool.onObjectSelected(object)
		end
	end
end

function tickTools(deltaTime)
	if EditorActiveTool ~= nil then
		if EditorActiveTool.onUpdate ~= nil then
			EditorActiveTool.onUpdate(deltaTime)
		end
	end
end