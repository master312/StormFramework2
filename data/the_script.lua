local this = {}


this.onUpdate = function(deltaTime) 
    if this.plane:containsPoint(InputManager:getPointerPosition(1)) then
        this.transform.angle = this.transform.angle + 1
    end
end

return this;

-- local this = {}

-- -- Called immidiaty after script loads. Method should be light as possible, sence it will slow loading
-- this.onLoad = function()
--     transform = this.transform
--     debug.log("Motherfucking shit 1 JEDAN 1 LOADED LOADED LOADED " .. this.id .. " " .. this.transform.posAbs:_str())
-- end 

-- -- Called after all scripts have been loaded, and ready to use
-- this.onStart = function()
--     debug.log("Started mother fucking shit! 777 " .. this.transform:getPosition():_str())
    
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