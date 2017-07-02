CC = g++
CFLAGS  = -g -Wall -std=c++11 -DSTORM_BUILD_PLATFORM_SDL2 #-pg
LIBS = -w -lSDL2 -lSDL2_image -lGLEW -lGL -lGLU 
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

# Scene stuff
$(OBJDIR)StormScene.o: src/scene/StormScene.cpp src/scene/StormScene.h
	$(CC) $(CFLAGS) -c src/scene/StormScene.cpp $(LIBS) -o $(OBJDIR)StormScene.o 
$(OBJDIR)StormSceneObject.o: src/scene/StormSceneObject.cpp src/scene/StormSceneObject.h
	$(CC) $(CFLAGS) -c src/scene/StormSceneObject.cpp $(LIBS) -o $(OBJDIR)StormSceneObject.o 
$(OBJDIR)SSceneComponent.o: src/scene/SSceneComponent.cpp src/scene/SSceneComponent.h
	$(CC) $(CFLAGS) -c src/scene/SSceneComponent.cpp $(LIBS) -o $(OBJDIR)SSceneComponent.o 
$(OBJDIR)SSceneComponentSystem.o: src/scene/SSceneComponentSystem.cpp src/scene/SSceneComponentSystem.h
	$(CC) $(CFLAGS) -c src/scene/SSceneComponentSystem.cpp $(LIBS) -o $(OBJDIR)SSceneComponentSystem.o 

# Components
$(OBJDIR)SSceneComPlane.o: src/scene/components/SSceneComPlane.cpp src/scene/components/SSceneComPlane.h
	$(CC) $(CFLAGS) -c src/scene/components/SSceneComPlane.cpp $(LIBS) -o $(OBJDIR)SSceneComPlane.o 
$(OBJDIR)SSceneComStaticTexture.o: src/scene/components/SSceneComStaticTexture.cpp src/scene/components/SSceneComStaticTexture.h
	$(CC) $(CFLAGS) -c src/scene/components/SSceneComStaticTexture.cpp $(LIBS) -o $(OBJDIR)SSceneComStaticTexture.o 

# Systems
$(OBJDIR)SSceneSystemStaticTexture.o: src/scene/components/SSceneSystemStaticTexture.cpp src/scene/components/SSceneSystemStaticTexture.h
	$(CC) $(CFLAGS) -c src/scene/components/SSceneSystemStaticTexture.cpp $(LIBS) -o $(OBJDIR)SSceneSystemStaticTexture.o 

debug: $(OBJDIR)easylogging++.o $(OBJDIR)pugixml.o $(OBJDIR)imgui.o $(OBJDIR)imgui_draw.o  $(OBJDIR)imgui_demo.o $(OBJDIR)StormEngine.o $(OBJDIR)StormPlatform.o $(OBJDIR)StormPlatformSDL2.o $(OBJDIR)StormInputManager.o $(OBJDIR)StormMiscTools.o $(OBJDIR)Plane.o $(OBJDIR)StormShader.o $(OBJDIR)StormTexture.o $(OBJDIR)StormVideoDriver.o $(OBJDIR)StormRenderer.o $(OBJDIR)StormResourceFile.o $(OBJDIR)StormTextureManager.o $(OBJDIR)StormFileSystem.o $(OBJDIR)StormTextureManager.o $(OBJDIR)StormSceneObject.o $(OBJDIR)StormScene.o $(OBJDIR)SSceneComponent.o $(OBJDIR)SSceneComponentSystem.o $(OBJDIR)SSceneComPlane.o $(OBJDIR)SSceneComStaticTexture.o $(OBJDIR)SSceneSystemStaticTexture.o
	$(CC) $(CFLAGS) $(OBJDIR)easylogging++.o $(OBJDIR)pugixml.o $(OBJDIR)imgui.o $(OBJDIR)imgui_draw.o $(OBJDIR)imgui_demo.o $(OBJDIR)StormEngine.o $(OBJDIR)StormPlatform.o $(OBJDIR)StormPlatformSDL2.o $(OBJDIR)StormInputManager.o $(OBJDIR)StormMiscTools.o $(OBJDIR)Plane.o $(OBJDIR)StormShader.o $(OBJDIR)StormTexture.o $(OBJDIR)StormVideoDriver.o $(OBJDIR)StormRenderer.o $(OBJDIR)StormResourceFile.o $(OBJDIR)StormFileSystem.o $(OBJDIR)StormTextureManager.o $(OBJDIR)StormSceneObject.o $(OBJDIR)StormScene.o $(OBJDIR)SSceneComponent.o $(OBJDIR)SSceneComponentSystem.o $(OBJDIR)SSceneComPlane.o $(OBJDIR)SSceneComStaticTexture.o $(OBJDIR)SSceneSystemStaticTexture.o main.cpp  -o "$(EXNAME)" $(LIBS)