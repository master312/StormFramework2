-- Transformation component QT widget script
-- used for editor

local this = {}

-- Called when widget gets created 
this.onCreated = function ()
	debug.log("Transform component widget onCreate!");

	local propPosition = EPropVec2.new(this.widget, "Position")
	local propScale = EPropVec2.new(this.widget, "Scale")

	-- propPosition:onValueChanged(function(newValue) 

	-- end)
end


this.onUpdate = function()
	-- debug.log("Com transform update")
end

return this