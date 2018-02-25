 
local this = {}

-- Name under which this tool is handled, something like tag
this.name = "position"
-- Label to be displayer in editor as tool name
this.label = "Positioning tool"


-- Called just after script gets loaded
this.onCreated = function()

end

-- Called when tool gets activated
this.onActivated = function()
	debug.log("--- Position TOOL activated")
end


-- Method is called every time selected object changes, and tool is left activated
this.onObjectSelected = function(object)
	debug.log("--- Position TOOL object selected")
end

-- Called when tool gets deactivated
this.onDeactivated = function()

end

-- Called every frame while tool is selected
this.onUpdate = function(deltaTime)

end

return this