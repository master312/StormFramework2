 
EditorUpdateDeltaTime = 0
EditorSceneObjectSelected = nil

-- Called from engine every time new scene object has been selected
function sceneObjectSelected(sceneObject)
	EditorSceneObjectSelected = sceneObject

	if toolHandleObjectSelected ~= nil then
		toolHandleObjectSelected(sceneObject)
	end
end

-- Called from object_handler.script to tick editor stuff
function tickEditor(deltaTime)

    EditorUpdateDeltaTime = EditorUpdateDeltaTime + deltaTime
    if EditorUpdateDeltaTime >= 0.25 then
    	-- Tick component widgets 4 times per second
    	EditorUpdateDeltaTime = 0
		tickComponentWidgets(deltaTime)
	end

	-- Ticks tools logic
	tickTools(deltaTime)
end