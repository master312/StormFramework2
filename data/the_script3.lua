local this = {}

this.onLoad = function()
    debug.log("LOADED SCRIPT 3. OBJ-ID: " .. this.id)
end


this.onUpdate = function(deltaTime) 
    local mousePosition = InputManager:getPointerPosition(1)
    if this.body:containsPoint(mousePosition) and InputManager:isPointerDown(1) then
        this.transform:setPosition(mousePosition);
    end
end

-- this.on_trigger_enter = function(object)
-- end

return this;