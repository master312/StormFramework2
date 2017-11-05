local this = {}

local isPressed = false
this.cnt = 0
this.onUpdate = function(deltaTime) 
    if InputManager:isKeyPressed(Keyboard.A) then
        this.cnt = 0
    end
    if this.cnt < 100 then
        this.cnt = this.cnt + 1
        this.transform:setPosition(InputManager:getPointerPosition(1))
    end
    -- debug.log(InputManager:getPointerPosition(1):toString())
end

return this;