#pragma once
#include <vector>
#include <map>
#include "StormSceneObject.h"
#include "SSceneComponentSystem.h"
#include "../core/resources/StormResourceFile.h"

class StormRenderer;

class StormScene {
public:
    StormScene();
    ~StormScene();

    /* Loads scene from XML file. 
     * Returns < 0 on error. */
    int loadXml(spStormResourceFile xmlFile);

    /* Saves scene to XML file.
     * If @customPath is not set, scene will be saved in /scenes folder 
     * in file named @_Name .xml */
    void saveXml(std::string path = "");

    /* Initialize all scene object components.
     * This method should be called once after scene has been loaded. */
    void initialize();

    /* Sets scene name */
    void setName(const std::string& name);

    /* Gets scene name */
    std::string getName() const;

    /* Adds new element to this scene. Ownership of the @object pointer
     * will be transfered to StormScene class, 
     * and memory will be freed when scene gets deleted */
    void addObject(StormSceneObject* object);

    /* Adds new object to scene, and returns pointer to it */
    StormSceneObject* addNewObject(const std::string& name = "");

    /* Renders scene */
    void render(StormRenderer* renderer);

    /* Ticks all scene logic */
    void tick(float deltaTime);

private:
    /* This variable increases every time new object is added.
     * It is used to assign unique ID to each scene object */
    uint32_t _LastObjectIndex;

    /* Vector containing all scene objects */
    std::vector<StormSceneObject*> _Objects;
    
    /* Vector containt systems for managing scene object component logic */
    std::vector<SSceneComponentSystem*> _ComponentSystems;
    
    /* All component systems indexed by their component types.
     * Used for faster access to systems. */
    std::map<SSceneComponentType, SSceneComponentSystem*> _ComponentSystemsByType;

    /* Secne name */
    std::string _Name;

    void initializeDefaultSystems();
};
