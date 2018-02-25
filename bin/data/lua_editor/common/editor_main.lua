 
EditorUpdateDeltaTime = 0

-- Called from object_handler.script to tick editor stuff
function tickEditor(deltaTime)

    EditorUpdateDeltaTime = EditorUpdateDeltaTime + deltaTime
    if EditorUpdateDeltaTime >= 0.25 then
    	EditorUpdateDeltaTime = 0
		tickComponentWidgets(deltaTime)
	end
end