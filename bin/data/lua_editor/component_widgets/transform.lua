-- Transformation component QT widget script
-- used for editor

local this = {}

-- Called when widget gets created 
this.onCreated = function ()
	debug.log("Transform component widget onCreate!");

	EPropVec2.new(this.widget, "Position")
	EPropVec2.new(this.widget, "Scale")
end


return this