local this = {}

-- local isPressed = false
-- this.toggle = false

this.onLoad = function()
    debug.log("THE LUA SCRIPT 2 LOAdED ON LOAD! " .. this.id)
end

this.onUpdate = function(deltaTime) 
    -- if InputManager:isKeyPressed(Keyboard.A) then
    --     -- this.toggle = not this.toggle
    --     this.body:applyForce(Vector2:new(200.0, -300.0));
    -- end

    -- if this.toggle then
    --     this.transform:setPosition(InputManager:getPointerPosition(1))
    -- end
    -- debug.log(InputManager:getPointerPosition(1):toString())
end

-- local inspect = require("lua_libs.inspect")
this.on_trigger_enter = function(object)
        
    debug.log("COLLIDED222 " .. object.id .. " SelfID " .. this.id)
end

return this;