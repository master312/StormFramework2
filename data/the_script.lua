local this = {}

local isPressed = false

this.onUpdate = function(deltaTime) 
    
    if this.plane:containsPoint(InputManager:getPointerPosition(1)) and InputManager:isPointerDown(1) then
        isPressed = true
    else
        if not InputManager:isPointerDown(1) then
            isPressed = false
        end
    end

    if this.plane:containsPoint(InputManager:getPointerPosition(1)) and InputManager:isPointerDown(2) then
        this.transform.angle = this.transform.angle + 1.2
    end
    

    if InputManager:isKeyPressed(Keyboard.A) then
        debug.log("A Key is pressed modafaka!")
    elseif InputManager:isKeyDown(Keyboard.B) then
        debug.log("Never forget B key")
    end

    if isPressed then
        local pointerPosition = InputManager:getPointerPosition(1)
        this.transform:setPosition(pointerPosition);
        -- debug.log("modafaka ")
    end

-- kerberovo sranje

    local position = this.transform.position
    -- debug.log(position:toString())	

end

return this;

-- local this = {}

-- -- Called immidiaty after script loads. Method should be light as possible, sence it will slow loading
-- this.onLoad = function()
--     transform = this.transform
--     debug.log("Modafakaaa 1 LOADED LOADED LOADED " .. this.id .. " " .. this.transform.posAbs:_str())
-- end 

-- -- Called after all scripts have been loaded, and ready to use
-- this.onStart = function()
--     debug.log("Started Fakamoda! 777 " .. this.transform:getPosition():_str())
    
--     if this.hasParent then
--         debug.log("HAS PARENT ");
--     else 
--         debug.log("NO HAS PARENT ");
--     end
-- end

-- this.onUpdate = function(deltaTime) 
--     local pointerPosition = InputManager:getPointerPosition(1)

--     if ComPlane.containsPoint(this.plane, pointerPosition) then
--         if InputManager:isPointerDown(1) then 
--             this.transform:setPosition(pointerPosition)
--         end
--         debug.log("MODAFAKA pointer containts " .. this.id)
--     end

--     -- if this.id == 4 then 
--     --     if InputManager:isPointerDown(1) then 
--     --         local pos = InputManager:getPointerPosition(1)
--     --         if this.hasParent then 
--     --             pos.x = pos.x - this.parent.obj.transform:getPosition().x
--     --             pos.y = pos.y - this.parent.obj.transform:getPosition().y
--     --         end
--     --         this.transform:setPosition(pos)
--     --     end
--     -- end 

--     if this.id ~= 1 then
--         this.transform.angle = this.transform.angle + 1
--     else
--         this.transform.angle = this.transform.angle + 1
--     end

--     -- if this.plane ~= nil then
--     --     if this.plane.isPressed then
--     --         this.transform.y = this.transform.y + 10
--     --     end
--     -- end
-- end

-- return this
