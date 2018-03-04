 
local this = {}

-- Name under which this tool is handled, something like tag
this.name = "position"
-- Label to be displayer in editor as tool name
this.label = "Positioning tool"

--- 0 - None
--- 1 - All directions
--- 2 - X axes
--- 3 - Y axes
this.moveDirection = 0
this.hasStartedMoving = false

-- Called just after script gets loaded
this.onCreated = function()
	
end

-- Called when tool gets activated
this.onActivated = function()
	this.hasStartedMoving = false
	this.moveDirection = 0

	debug.log("--- Position TOOL activated")
end


-- Method is called every time selected object changes, and tool is left activated
this.onObjectSelected = function(object)
	debug.log("--- Position TOOL object selected")
end

-- Called when tool gets deactivated
this.onDeactivated = function()
	QtApplication.setOverrideCursor(QtApplication.ArrowCursor)
end

this.handleMovementAction = function()
	local pointerPos = InputManager:getPointerPosition(1)
	local comTransform = EditorSceneObjectSelected.transform

	if this.moveDirection == 1 then
		-- Move in all directions
		comTransform.position = comTransform.position + (pointerPos - this.lastPos)
	elseif this.moveDirection == 2 then 
		-- Move only on X
		local newPos = comTransform.position
		newPos.x = newPos.x + (pointerPos.x - this.lastPos.x)
		comTransform:setPosition(newPos)
	elseif this.moveDirection == 3 then 
		-- Move only on Y
		local newPos = comTransform.position
		newPos.y = newPos.y + (pointerPos.y - this.lastPos.y)
		comTransform:setPosition(newPos)
	end

	this.lastPos = pointerPos

	if not InputManager:isPointerDown(1) then
		this.hasStartedMoving = false
	end
end

-- Called every frame while tool is selected
this.onUpdate = function(deltaTime)
	if not EditorSceneObjectSelected then return end

	if this.hasStartedMoving then 
		-- Currently moving.
		this.handleMovementAction()
		return 
	end

	-- No movement is active.
	-- Choose proper direction here
	local objPosition = EditorSceneObjectSelected.getPosAbs()
	local distanceVec = InputManager:getPointerPosition(1) - objPosition
	distanceVec = distanceVec:abs()
	
	if distanceVec.x < 0.15 and distanceVec.y < 0.15 then 
		-- Move in all directions
		this.moveDirection = 1
		QtApplication.setOverrideCursor(QtApplication.SizeAllCursor)
	elseif distanceVec.x >= 0.15 and distanceVec.y < 0.15 then
		-- Move on X axes
		this.moveDirection = 2
		QtApplication.setOverrideCursor(QtApplication.SizeHorCursor)
	elseif distanceVec.x < 0.15 and distanceVec.y >= 0.15 then
		-- Move on Y axes
		this.moveDirection = 3
		QtApplication.setOverrideCursor(QtApplication.SizeVerCursor)
	else
		-- Don't move
		this.moveDirection = 0
		QtApplication.setOverrideCursor(QtApplication.ArrowCursor)
	end

	if InputManager:isPointerDown(1) and this.moveDirection ~= 0 then
		-- We have direction and pointer is pressed. Start moving the object
		this.hasStartedMoving = true
		this.lastPos = InputManager:getPointerPosition(1)
	end
end

return this