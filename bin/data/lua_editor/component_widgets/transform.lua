-- Transformation component QT widget script
-- used for editor

local this = {}

-- Component properties
propPosition = {}
propScale = {}
propAngle = {}

-- Called when widget gets created 
this.onCreated = function ()
	local root = this.widget

	debug.log("Transform component widget onCreate!");

	root:setName("Transform")

	propPosition = EPropVec2.new(root, "Position")
	propPosition:onValueChanged(function(newValue) 
		-- Callback when value in property change
		this.component:setPosition(newValue);
	end)


	propScale = EPropVec2.new(root, "Scale")
	propScale:onValueChanged(function(newValue)
		this.component:setScale(newValue)
	end)


	propAngle = EPropNumber.new(root, "Angle", "angle")
	propAngle:onValueChanged(function(newValue)
		this.component:setAngle(newValue)
	end)
end


this.onUpdate = function()
	propPosition.value = this.component.position
	propScale.value = this.component.scale
	propAngle.value = this.component.angle

	-- debug.log("Com transform update")
end

return this