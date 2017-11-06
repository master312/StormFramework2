local this = {}

this.onLoad = function()
    debug.log("LOADED SCRIPT 3. OBJ-ID: " .. this.id)
end


this.onUpdate = function(deltaTime) 
    local mousePosition = InputManager:getPointerPosition(1)
    if this.body:containsPoint(mousePosition) then
        if InputManager:isPointerDown(1) then 
            this.body:applyLinearImpulse(Vector2:new(1.0, -1.0));
        elseif InputManager:isPointerDown(2) then
            this.body:applyLinearImpulse(Vector2:new(-1.0, -1.0));
        end
    end
end

-- this.on_trigger_enter = function(object)
-- end

return this;