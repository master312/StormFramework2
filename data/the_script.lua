local this = {}

-- Called immidiaty after script loads. Method should be light as possible, sence it will slow loading
this.onLoad = function()
    transform = this.transform
    debug.log("Motherfucking shit 1 JEDAN 1 LOADED LOADED LOADED " .. this.id .. " " .. this.transform.posAbs:_str())
end 

-- Called after all scripts have been loaded, and ready to use
this.onStart = function()
    debug.log("Started mother fucking shit! 777 " .. this.transform.pos:_str())
end

this.onUpdate = function(deltaTime) 
    local tmp = this.transform.pos
    tmp.x = tmp.x - 1
    this.transform.pos.x = tmp.x
    debug.log("Aaa " .. this.transform.pos:_str())
end

return this

