#pragma once
#include <vector>
#include <map>
#include "StormSceneObject.h"
#include "SSceneComponentSystem.h"
#include "resources/StormResourceFile.h"

class StormRenderer;

/* TODO - OPTIMIZATION: Split components into 2 vectors. Tickable and untickable, so we dont 
 * have to iterate trought all components on render if some of them are unrenerable. */

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

    /* Initialize all scene object component systems.
     * This method should be called once after scene has been loaded. */
    void initialize();

    /* Sets scene name */
    void setName(const std::string& name);

    /* Gets scene name */
    std::reference_wrapper<const std::string> getName() const;

    /* Returns true if scene is initialized */
    bool isInitialized();

    /* Adds new element to this scene. Ownership of the @object pointer
     * will be transfered to StormScene class, 
     * and memory will be freed when scene gets deleted */
    void addObject(StormSceneObject* object);

    /* Creates new script object from prefab, and return pointer to it.
     * Returns nullptr on error */
    StormSceneObject* instantiatePrefab(const std::string& prefabName, 
                                        const std::string& objectName = "");

    /* Returns vector of all scene objects */
    std::vector<StormSceneObject*>& getObjects();

    /* Returns pointer to object with ID */
    StormSceneObject* getObjectById(uint32_t id);

    /* Retnurns reference to rector containing all component systems */
    std::vector<SSceneComponentSystem*>& getSystems();

    /* Returns reference to map containing all prefabs. */
    std::map<std::string, pugi::xml_node>& getPrefabs();

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
    
    /* Vector containt various systems for managing scene object component logic */
    std::vector<SSceneComponentSystem*> _ComponentSystems;
    
    /* All component systems indexed by their component types.
    * Used for faster access to systems. */
    SSceneComponentSystem* _ComponentSystemsByType[S_SCENE_OBJECT_COM_TYPES_COUNT];
    
    /* Document from which this scene has been loaded */
    pugi::xml_document _XmlDocument;

    /* Map of all object prefabs indexed by their names */
    std::map<std::string, pugi::xml_node> _Prefabs;

    /* Secne name */
    std::string _Name;

    /* Holds the greatest object ID */
    uint32_t _MaxObjectId;

    /* Is scene been initialized */
    bool _IsInitialized;
    
    /* Used for initializing objects added at runtime */
    void initializeObject(StormSceneObject* object);

    void initializeDefaultSystems();
};
