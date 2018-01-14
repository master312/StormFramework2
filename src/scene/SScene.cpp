#include "SScene.h"
#include "SSceneObject.h"

#include "components/physics/SSceneSystemPhysics.h"
#include "components/sprite/SSceneSystemSprite.h"
#include "components/transform/SSceneSystemTransform.h"
#include "components/luaScript/SSceneSystemLuaScript.h"

#include "StormEngine.h"
#include "graphics/StormRenderer.h"

SScene::SScene() {
    _LastObjectIndex = 1;
    _Name = "";
    _IsInitialized = false;
    _MaxObjectId = 0;

    /* Fills @_ComponentSystemsByType array with NULL */
    memset(_ComponentSystemsByType, 0, S_SCENE_OBJECT_COM_TYPES_COUNT * sizeof(SSceneComponentSystem*));

    initializeDefaultSystems();
}

SScene::~SScene() {
    for (size_t i = 0; i < _Objects.size(); i++) {
        delete _Objects[i];
    }
    _Objects.clear();

    for (size_t i = 0; i < _ComponentSystems.size(); i++) {
        delete _ComponentSystems[i];
    }
    _ComponentSystems.clear();
    memset(_ComponentSystemsByType, 0, S_SCENE_OBJECT_COM_TYPES_COUNT * sizeof(SSceneComponentSystem*));
}

int SScene::loadXml(spStormResourceFile xmlFile) {
    pugi::xml_parse_result result = _XmlDocument.load(xmlFile->getBuffer(), xmlFile->getBufferSize());
    if (result.status != pugi::status_ok) {
        LOG(ERROR) << "Scene XML " << xmlFile->getFilename() << " error: " << result.description();
        return -1;
    }
    
    pugi::xml_node sceneRootNode = _XmlDocument.child("scene");
    if (sceneRootNode.type() != pugi::node_element) {
        LOG(ERROR) << "Scene XML " << xmlFile->getFilename() << " error. Missing root tag.";
        return -2;
    }
    _Name = sceneRootNode.attribute("name").as_string("");

    /* Map used loading linked objects
     * <objectId, parentId> */
    std::map<uint32_t, uint32_t> hierarchy;
    for (pugi::xml_node objectNode = sceneRootNode.first_child(); objectNode; objectNode = objectNode.next_sibling()) {
        SSceneObject* object = new SSceneObject(this);
        if (object->deserializeXml(objectNode) < 0) {
            LOG(ERROR) << "Object XML deserialization error";
            delete object;
            continue;
        }
        int parentId = objectNode.attribute("parent").as_int(-1);
        if (parentId > 0) {
            /* This object has parent set */
            hierarchy[object->getId()] = (uint32_t) parentId;
        }

        addObject(object);
    }

    pugi::xml_node prefabsRootNode = _XmlDocument.child("prefabs");
    for (pugi::xml_node prefabNode = prefabsRootNode.first_child(); prefabNode; prefabNode = prefabNode.next_sibling()) {
        /* This object is used as prefab. Dont load it */
        std::string name = prefabNode.attribute("name").as_string();
        if (!name.size()) {
            LOG(ERROR) << "Prefab in scene file dose not have a name. It won be abe do instantiate";
            continue;
        }
        _Prefabs[name] = prefabNode;
        continue;
    }

    /* Assign parents objects */
    for (auto& iter : hierarchy) {
        if (iter.first == iter.second) {
            LOG(ERROR) << "Error in scene XML file. Object " << iter.first << " have it self as parent.";
            continue;
        }
        SSceneObject* child = getObjectById(iter.first);
        SSceneObject* parent = getObjectById(iter.second);
        if (!child) {
            LOG(ERROR) << "Error in scene XML file. Object " << iter.first << " not found.";
            continue;
        }
        if (!parent) {
            LOG(ERROR) << "Error in scene XML file. Parent object " << iter.second << " not found.";
            continue;
        }
        child->setParent(parent);
    }


    LOG(INFO) << "Scene '" << _Name << "' loaded";
    return 1;
}

void SScene::saveXml(std::string path /* = "" */) {
    if (path == "") {
        if (_Name != "") {
            path = "scenes/" + _Name + ".xml";
            std::transform(path.begin(), path.end(), path.begin(), ::tolower);
        } else {
            path = "scenes/unnamed_scene.xml";
        }
    }
    
    pugi::xml_document doc;
    pugi::xml_node sceneRootNode = doc.append_child("scene");

    sceneRootNode.append_attribute("name").set_value(_Name.c_str());

    for (SSceneObject* object : _Objects) {
        pugi::xml_node objectNode = sceneRootNode.append_child("object");
        object->serializeXml(objectNode);
    }

    pugi::xml_node prefabRootNode = doc.append_child("prefabs");
    for (auto& iter : _Prefabs) {
        prefabRootNode.append_copy(iter.second);
    }

    std::stringstream ss;
    doc.save(ss);
    
    std::string xmlBuffer = ss.str();

    spStormResourceFile file = new StormResourceFile(path);
    file->allocateBuffer(xmlBuffer.size(), (char*)xmlBuffer.c_str());

    if (StormEngine::getModule<StormFileSystem>()->saveResourceFile(file) > 0) {
        LOG(INFO) << "Scene '" << _Name << "' saved to xml";
    } else {
        LOG(DEBUG) << "Error occured while saving scene'" << _Name << "'";
    }
}

void SScene::initialize() {
    for (int i = 0; i < S_SCENE_OBJECT_COM_TYPES_COUNT; i++) {
        int nextToInit = SSceneComponentInitializationOrder[i];
        if (_ComponentSystemsByType[nextToInit]) {
            /* Component system exists */
            _ComponentSystemsByType[nextToInit]->initialize();
        }
    }
    
    LOG(INFO) << "Scene '" << _Name << "' component systems initialized";
    _IsInitialized = true;
}

void SScene::setName(const std::string& name) {
    _Name = name;
}

std::reference_wrapper<const std::string> SScene::getName() const {
    return _Name;
}

bool SScene::isInitialized() {
    return _IsInitialized;
}

void SScene::addObject(SSceneObject* object) {
    for (SSceneComponent* com : object->getComponents()) {
        SSceneComponentSystem* comSystem = _ComponentSystemsByType[com->getType()];
        if (comSystem) {
            /* System for this component type exists */
            comSystem->addComponent(com);
        }
    }

    if (object->getId() > _MaxObjectId) {
        _MaxObjectId = object->getId();
    }
    _Objects.push_back(object);
}

SSceneObject* SScene::instantiatePrefab(const std::string& prefabName,
                                                const std::string& objectName /* = "" */) {
    if (prefabName == "") {
        return nullptr;
    }
    auto iter = _Prefabs.find(prefabName);
    if (iter == _Prefabs.end()) {
        LOG(ERROR) << "Could not instantiate prefab '" << prefabName << "'. It dose not exist";
        return nullptr;
    }
    pugi::xml_node prefabNode = iter->second;
    
    SSceneObject* object = new SSceneObject(this);
    if (object->deserializeXml(prefabNode) < 0) {
        LOG(ERROR) << "Object XML deserialization error";
        delete object;
        return nullptr;
    }

    _MaxObjectId++;
    object->setIsCreatedAtRuntime(true);
    object->setId(_MaxObjectId);
    object->setName(objectName);

    addObject(object);
    initializeNewObject(object);
    
    LOG(DEBUG) << "Instantiated new object from prefab '" << prefabName << "'. ID: " << _MaxObjectId;

    return object;
}

void SScene::initializeNewObject(SSceneObject* object) {
    /* Loop initializes all components */
    static std::vector<SSceneComponentSystem*> addedSystems;
    addedSystems.clear();
    for (int i = 0; i < S_SCENE_OBJECT_COM_TYPES_COUNT; i++) {
        int nextToInit = SSceneComponentInitializationOrder[i];
        SSceneComponent* component = object->getComponent((SSceneComponentType)nextToInit);
        if (component) {
            SSceneComponentSystem* comSystem = _ComponentSystemsByType[nextToInit];
            component->initialize(comSystem);
            addedSystems.push_back(comSystem);
        }
    }

    /* Now bind all components to lua script */
    for (SSceneComponentSystem* system : addedSystems) {
        system->bindComponentsToLua(getScriptSystem());
    }
}

std::vector<SSceneObject*>& SScene::getObjects() {
    return _Objects;
}

SSceneObject* SScene::getObjectById(uint32_t id) {
    for (SSceneObject* object : _Objects) {
        if (object->getId() == id) {
            return object;
        }
    }
    return nullptr;
}

std::vector<SSceneComponentSystem*>& SScene::getSystems() {
    return _ComponentSystems;
}

SSceneComponentSystem* SScene::getSystemByType(SSceneComponentType type) {
    return _ComponentSystemsByType[(int)type];
}

SSceneSystemLuaScript* SScene::getScriptSystem() {
    return dynamic_cast<SSceneSystemLuaScript*>(getSystemByType(S_SCENE_OBJECT_COM_SCRIPT));
}

std::map<std::string, pugi::xml_node>& SScene::getPrefabs() {
    return _Prefabs;
}

void SScene::render(StormRenderer* renderer) {
    for (unsigned int i = 0; i < _ComponentSystems.size(); i++) {
        _ComponentSystems[i]->render(renderer);
    }
}

void SScene::tick(float deltaTime) {
    for (unsigned int i = 0; i < SSceneComponentTickingOrderCount; i++) {
        int typeToTick = SSceneComponentTickingOrder[i];
        if (_ComponentSystemsByType[typeToTick]) {
            _ComponentSystemsByType[typeToTick]->tick(deltaTime);
        }
    }
}

void SScene::initializeDefaultSystems() {
    SSceneSystemTransform* sysTransform = new SSceneSystemTransform(this);
    _ComponentSystems.push_back(sysTransform);
    _ComponentSystemsByType[S_SCENE_OBJECT_COM_TRANSFORM] = sysTransform;

    SSceneSystemSprite* sysSprite = new SSceneSystemSprite(this);
    _ComponentSystems.push_back(sysSprite);
    _ComponentSystemsByType[S_SCENE_OBJECT_COM_SPRITE] = sysSprite;

    SSceneSystemPhysics* sysCollider = new SSceneSystemPhysics(this);
    _ComponentSystems.push_back(sysCollider);
    _ComponentSystemsByType[S_SCENE_OBJECT_COM_PHYSICS] = sysCollider;

    SSceneSystemLuaScript* sysLua = new SSceneSystemLuaScript(this);
    _ComponentSystems.push_back(sysLua);
    _ComponentSystemsByType[S_SCENE_OBJECT_COM_SCRIPT] = sysLua;
}
