CC = g++
CFLAGS  = -g -Wall -std=c++14 -DSTORM_BUILD_PLATFORM_SDL2 -D_DEBUG -I"/usr/include/lua5.1/" -I"/usr/lib/" -I"libs/sol2/" #-pg
LIBS = -w -lSDL2 -lSDL2_image -lGLEW -lGL -lGLU -llua5.1
OBJDIR = objects/
EXNAME = a.out

default: debug

# Libs
$(OBJDIR)easylogging++.o: libs/easyloggingpp/easylogging++.cpp libs/easyloggingpp/easylogging++.h
	$(CC) $(CFLAGS) -c libs/easyloggingpp/easylogging++.cpp $(LIBS) -o $(OBJDIR)easylogging++.o 
$(OBJDIR)pugixml.o: libs/pugixml/pugixml.cpp libs/pugixml/pugixml.hpp
	$(CC) $(CFLAGS) -c libs/pugixml/pugixml.cpp $(LIBS) -o $(OBJDIR)pugixml.o 
$(OBJDIR)imgui.o: libs/imgui/imgui.cpp libs/imgui/imgui.h
	$(CC) $(CFLAGS) -c libs/imgui/imgui.cpp $(LIBS) -o $(OBJDIR)imgui.o 
$(OBJDIR)imgui_draw.o: libs/imgui/imgui_draw.cpp
	$(CC) $(CFLAGS) -c libs/imgui/imgui_draw.cpp $(LIBS) -o $(OBJDIR)imgui_draw.o 
$(OBJDIR)imgui_demo.o: libs/imgui/imgui_demo.cpp
	$(CC) $(CFLAGS) -c libs/imgui/imgui_demo.cpp $(LIBS) -o $(OBJDIR)imgui_demo.o 


$(OBJDIR)StormPlatform.o: src/core/platforms/StormPlatform.cpp src/core/platforms/StormPlatform.h
	$(CC) $(CFLAGS) -c src/core/platforms/StormPlatform.cpp $(LIBS) -o $(OBJDIR)StormPlatform.o 
$(OBJDIR)StormPlatformSDL2.o: src/core/platforms/StormPlatformSDL2.cpp src/core/platforms/StormPlatformSDL2.h
	$(CC) $(CFLAGS) -c src/core/platforms/StormPlatformSDL2.cpp $(LIBS) -o $(OBJDIR)StormPlatformSDL2.o 
$(OBJDIR)StormInputManager.o: src/core/platforms/StormInputManager.cpp src/core/platforms/StormInputManager.h
	$(CC) $(CFLAGS) -c src/core/platforms/StormInputManager.cpp $(LIBS) -o $(OBJDIR)StormInputManager.o 
$(OBJDIR)Plane.o: src/core/utils/Plane.cpp src/core/utils/Plane.h
	$(CC) $(CFLAGS) -c src/core/utils/Plane.cpp $(LIBS) -o $(OBJDIR)Plane.o 
$(OBJDIR)Geometry.o: src/core/utils/Geometry.cpp src/core/utils/Geometry.h
	$(CC) $(CFLAGS) -c src/core/utils/Geometry.cpp $(LIBS) -o $(OBJDIR)Geometry.o 
$(OBJDIR)StormMiscTools.o: src/core/utils/StormMiscTools.cpp src/core/utils/StormMiscTools.h
	$(CC) $(CFLAGS) -c src/core/utils/StormMiscTools.cpp $(LIBS) -o $(OBJDIR)StormMiscTools.o 
$(OBJDIR)StormShader.o: src/core/graphics/StormShader.cpp src/core/graphics/StormShader.h
	$(CC) $(CFLAGS) -c src/core/graphics/StormShader.cpp $(LIBS) -o $(OBJDIR)StormShader.o 
$(OBJDIR)StormTexture.o: src/core/graphics/StormTexture.cpp src/core/graphics/StormTexture.h
	$(CC) $(CFLAGS) -c src/core/graphics/StormTexture.cpp $(LIBS) -o $(OBJDIR)StormTexture.o 
$(OBJDIR)StormVideoDriver.o: src/core/graphics/StormVideoDriver.cpp src/core/graphics/StormVideoDriver.h
	$(CC) $(CFLAGS) -c src/core/graphics/StormVideoDriver.cpp $(LIBS) -o $(OBJDIR)StormVideoDriver.o 
$(OBJDIR)StormRenderer.o: src/core/graphics/StormRenderer.cpp src/core/graphics/StormRenderer.h
	$(CC) $(CFLAGS) -c src/core/graphics/StormRenderer.cpp $(LIBS) -o $(OBJDIR)StormRenderer.o 
$(OBJDIR)StormResourceFile.o: src/core/resources/StormResourceFile.cpp src/core/resources/StormResourceFile.h
	$(CC) $(CFLAGS) -c src/core/resources/StormResourceFile.cpp $(LIBS) -o $(OBJDIR)StormResourceFile.o 
$(OBJDIR)StormFileSystem.o: src/core/resources/StormFileSystem.cpp src/core/resources/StormFileSystem.h
	$(CC) $(CFLAGS) -c src/core/resources/StormFileSystem.cpp $(LIBS) -o $(OBJDIR)StormFileSystem.o 

# Engine stuff
$(OBJDIR)StormTextureManager.o: src/StormTextureManager.cpp src/StormTextureManager.h
	$(CC) $(CFLAGS) -c src/StormTextureManager.cpp $(LIBS) -o $(OBJDIR)StormTextureManager.o 
$(OBJDIR)StormEngine.o: src/StormEngine.cpp src/StormEngine.h
	$(CC) $(CFLAGS) -c src/StormEngine.cpp $(LIBS) -o $(OBJDIR)StormEngine.o 
$(OBJDIR)SEngineModuleFactory.o: src/SEngineModuleFactory.cpp src/SEngineModuleFactory.h
	$(CC) $(CFLAGS) -c src/SEngineModuleFactory.cpp $(LIBS) -o $(OBJDIR)SEngineModuleFactory.o 

# Scene stuff
$(OBJDIR)StormScene.o: src/scene/StormScene.cpp src/scene/StormScene.h
	$(CC) $(CFLAGS) -c src/scene/StormScene.cpp $(LIBS) -o $(OBJDIR)StormScene.o 
$(OBJDIR)SSceneManager.o: src/scene/SSceneManager.cpp src/scene/SSceneManager.h
	$(CC) $(CFLAGS) -c src/scene/SSceneManager.cpp $(LIBS) -o $(OBJDIR)SSceneManager.o 
$(OBJDIR)StormSceneObject.o: src/scene/StormSceneObject.cpp src/scene/StormSceneObject.h
	$(CC) $(CFLAGS) -c src/scene/StormSceneObject.cpp $(LIBS) -o $(OBJDIR)StormSceneObject.o 
$(OBJDIR)SSceneComponent.o: src/scene/SSceneComponent.cpp src/scene/SSceneComponent.h
	$(CC) $(CFLAGS) -c src/scene/SSceneComponent.cpp $(LIBS) -o $(OBJDIR)SSceneComponent.o 
$(OBJDIR)SSceneComponentSystem.o: src/scene/SSceneComponentSystem.cpp src/scene/SSceneComponentSystem.h
	$(CC) $(CFLAGS) -c src/scene/SSceneComponentSystem.cpp $(LIBS) -o $(OBJDIR)SSceneComponentSystem.o 

# Components
$(OBJDIR)SSceneComTransform.o: src/scene/components/SSceneComTransform.cpp src/scene/components/SSceneComTransform.h
	$(CC) $(CFLAGS) -c src/scene/components/SSceneComTransform.cpp $(LIBS) -o $(OBJDIR)SSceneComTransform.o 
$(OBJDIR)SSceneComSprite.o: src/scene/components/SSceneComSprite.cpp src/scene/components/SSceneComSprite.h
	$(CC) $(CFLAGS) -c src/scene/components/SSceneComSprite.cpp $(LIBS) -o $(OBJDIR)SSceneComSprite.o 
$(OBJDIR)SSceneComLuaScript.o: src/scene/components/SSceneComLuaScript.cpp src/scene/components/SSceneComLuaScript.h
	$(CC) $(CFLAGS) -c src/scene/components/SSceneComLuaScript.cpp $(LIBS) -o $(OBJDIR)SSceneComLuaScript.o 
$(OBJDIR)SSceneComCollider.o: src/scene/components/SSceneComCollider.cpp src/scene/components/SSceneComCollider.h
	$(CC) $(CFLAGS) -c src/scene/components/SSceneComCollider.cpp $(LIBS) -o $(OBJDIR)SSceneComCollider.o 

# Lua binding class
$(OBJDIR)SLuaBinders.o: src/scene/components/lua/SLuaBinders.cpp src/scene/components/lua/SLuaBinders.h
	$(CC) $(CFLAGS) -c src/scene/components/lua/SLuaBinders.cpp $(LIBS) -o $(OBJDIR)SLuaBinders.o 

# Systems
$(OBJDIR)SSceneSystemSprite.o: src/scene/components/SSceneSystemSprite.cpp src/scene/components/SSceneSystemSprite.h
	$(CC) $(CFLAGS) -c src/scene/components/SSceneSystemSprite.cpp $(LIBS) -o $(OBJDIR)SSceneSystemSprite.o 
$(OBJDIR)SSceneSystemTransform.o: src/scene/components/SSceneSystemTransform.cpp src/scene/components/SSceneSystemTransform.h
	$(CC) $(CFLAGS) -c src/scene/components/SSceneSystemTransform.cpp $(LIBS) -o $(OBJDIR)SSceneSystemTransform.o 
$(OBJDIR)SSceneSystemLuaScript.o: src/scene/components/SSceneSystemLuaScript.cpp src/scene/components/SSceneSystemLuaScript.h
	$(CC) $(CFLAGS) -c src/scene/components/SSceneSystemLuaScript.cpp $(LIBS) -o $(OBJDIR)SSceneSystemLuaScript.o 
$(OBJDIR)SSceneSystemCollider.o: src/scene/components/SSceneSystemCollider.cpp src/scene/components/SSceneSystemCollider.h
	$(CC) $(CFLAGS) -c src/scene/components/SSceneSystemCollider.cpp $(LIBS) -o $(OBJDIR)SSceneSystemCollider.o 

debug: $(OBJDIR)easylogging++.o $(OBJDIR)pugixml.o $(OBJDIR)imgui.o $(OBJDIR)imgui_draw.o $(OBJDIR)SSceneSystemLuaScript.o $(OBJDIR)SSceneComLuaScript.o $(OBJDIR)SSceneComCollider.o $(OBJDIR)SSceneSystemCollider.o $(OBJDIR)imgui_demo.o $(OBJDIR)SEngineModuleFactory.o $(OBJDIR)StormEngine.o $(OBJDIR)StormPlatform.o $(OBJDIR)StormPlatformSDL2.o $(OBJDIR)StormInputManager.o $(OBJDIR)StormMiscTools.o $(OBJDIR)Plane.o $(OBJDIR)Geometry.o $(OBJDIR)StormShader.o $(OBJDIR)StormTexture.o $(OBJDIR)StormVideoDriver.o $(OBJDIR)StormRenderer.o $(OBJDIR)StormResourceFile.o $(OBJDIR)StormTextureManager.o $(OBJDIR)StormFileSystem.o $(OBJDIR)StormTextureManager.o $(OBJDIR)StormSceneObject.o $(OBJDIR)SSceneComTransform.o $(OBJDIR)StormScene.o $(OBJDIR)SSceneManager.o $(OBJDIR)SSceneComponent.o $(OBJDIR)SSceneComponentSystem.o $(OBJDIR)SSceneComSprite.o $(OBJDIR)SSceneSystemSprite.o $(OBJDIR)SSceneSystemTransform.o $(OBJDIR)SLuaBinders.o src/core/utils/SObservable.h
	$(CC) $(CFLAGS) $(OBJDIR)easylogging++.o $(OBJDIR)pugixml.o $(OBJDIR)imgui.o $(OBJDIR)SSceneSystemLuaScript.o $(OBJDIR)SSceneComLuaScript.o $(OBJDIR)SSceneComCollider.o $(OBJDIR)SSceneSystemCollider.o $(OBJDIR)imgui_draw.o $(OBJDIR)imgui_demo.o $(OBJDIR)SEngineModuleFactory.o $(OBJDIR)StormEngine.o $(OBJDIR)StormPlatform.o $(OBJDIR)StormPlatformSDL2.o $(OBJDIR)StormInputManager.o $(OBJDIR)StormMiscTools.o $(OBJDIR)Plane.o $(OBJDIR)Geometry.o $(OBJDIR)StormShader.o $(OBJDIR)StormTexture.o $(OBJDIR)StormVideoDriver.o $(OBJDIR)StormRenderer.o $(OBJDIR)StormResourceFile.o $(OBJDIR)StormFileSystem.o $(OBJDIR)StormTextureManager.o $(OBJDIR)StormSceneObject.o $(OBJDIR)SSceneComTransform.o $(OBJDIR)StormScene.o $(OBJDIR)SSceneManager.o $(OBJDIR)SSceneComponent.o $(OBJDIR)SSceneComponentSystem.o $(OBJDIR)SSceneComSprite.o $(OBJDIR)SSceneSystemSprite.o $(OBJDIR)SSceneSystemTransform.o $(OBJDIR)SLuaBinders.o main.cpp  -o "$(EXNAME)" $(LIBS)