local this = {}

local isPressed = false
this.toggle = false

this.onUpdate = function(deltaTime) 
    if InputManager:isKeyPressed(Keyboard.A) then
        -- this.toggle = not this.toggle
        this.body:applyForce(Vector2:new(200.0, -300.0));
    end

    if this.toggle then
        this.transform:setPosition(InputManager:getPointerPosition(1))
    end
    -- debug.log(InputManager:getPointerPosition(1):toString())
end

this.on_trigger_enter = function(object)
    debug.log("COLLIDED " .. object.id)
end

return this;