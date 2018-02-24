-- Transformation component QT widget script
-- used for editor

local this = {}

-- Called when widget gets created 
this.onCreated = function ()
	debug.log("Transform component widget onCreate!");

	this.propPosition = EPropVec2.new(this.widget, "Position")
	this.propPosition:onValueChanged(function(newValue) 
		-- Callback when value in property change
		this.component:setPosition(newValue);
		debug.log("changedddd")
	end)


	this.propScale = EPropVec2.new(this.widget, "Scale")
	this.propScale:onValueChanged(function(newValue)
		this.component:setScale(newValue)
	end)

end


this.onUpdate = function()
	this.propPosition.value = this.component.position
	this.propScale.value = this.component.scale

	-- debug.log("Com transform update")
end

return this