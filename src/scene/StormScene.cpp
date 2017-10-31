#include "StormScene.h"
#include "StormSceneObject.h"

#include "components/SSceneSystemPlane.h"
#include "components/SSceneSystemSprite.h"
#include "components/SSceneSystemTransform.h"
#include "components/SSceneSystemLuaScript.h"

#include "../StormEngine.h"
#include "../core/graphics/StormRenderer.h"
#include "../core/resources/StormFileSystem.h"

StormScene::StormScene() {
    _LastObjectIndex = 1;
    _Name = "";
    _IsInitialized = false;
    
    /* Fills @_ComponentSystemsByType array with NULL */
    memset(_ComponentSystemsByType, 0, S_SCENE_OBJECT_COM_TYPES_COUNT * sizeof(SSceneComponentSystem*));

    initializeDefaultSystems();
}

StormScene::~StormScene() {
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

int StormScene::loadXml(spStormResourceFile xmlFile) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load(xmlFile->getBuffer(), xmlFile->getBufferSize());
    if (result.status != pugi::status_ok) {
        LOG(ERROR) << "Scene XML " << xmlFile->getFilename() << " error: " << result.description();
        return -1;
    }
    
    pugi::xml_node sceneRootNode = doc.child("scene");
    if (sceneRootNode.type() != pugi::node_element) {
        LOG(ERROR) << "Scene XML " << xmlFile->getFilename() << " error. Missing root tag.";
        return -2;
    }
    _Name = sceneRootNode.attribute("name").as_string("");

    /* Map used loading linked objects
     * <objectId, parentId> */
    std::map<uint32_t, uint32_t> hierarchy;
    for (pugi::xml_node objectNode = sceneRootNode.first_child(); objectNode; objectNode = objectNode.next_sibling()) {
        StormSceneObject* object = new StormSceneObject();
        if (object->deserializeXml(objectNode) < 0) {
            LOG(ERROR) << "Object XML deserialization error";
            continue;
        }
        int parentId = objectNode.attribute("parent").as_int(-1);
        if (parentId > 0) {
            /* This object has parent set */
            hierarchy[object->getId()] = (uint32_t) parentId;
        }

        addObject(object);
    }

    /* Assign parents objects */
    for (auto& iter : hierarchy) {
        if (iter.first == iter.second) {
            LOG(ERROR) << "Error in scene XML file. Object " << iter.first << " have it self as parent.";
            continue;
        }
        StormSceneObject* child = getObjectById(iter.first);
        StormSceneObject* parent = getObjectById(iter.second);
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

void StormScene::saveXml(std::string path /* = "" */) {
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

    for (StormSceneObject* object : _Objects) {
        pugi::xml_node objectNode = sceneRootNode.append_child("object");
        object->serializeXml(objectNode);
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

void StormScene::initialize() {
    for (int i = 0; i < S_SCENE_OBJECT_COM_TYPES_COUNT; i++) {
        int nextToInit = SSceneComponentInitializationOrder[i];
        if (_ComponentSystemsByType[nextToInit]) {
            /* Component system exists */
            _ComponentSystemsByType[nextToInit]->initialize(this);
        }
    }
    
    _IsInitialized = true;
    LOG(INFO) << "Scene '" << _Name << "' component systems initialized";
}

void StormScene::setName(const std::string& name) {
    _Name = name;
}

std::string StormScene::getName() const {
    return _Name;
}

bool StormScene::isInitialized() {
    return _IsInitialized;
}

void StormScene::addObject(StormSceneObject* object) {
    for (SSceneComponent* com : object->getComponents()) {
        SSceneComponentSystem* comSystem = _ComponentSystemsByType[com->getType()];
        if (comSystem) {
            /* System for this component type exists */
            comSystem->addComponent(com);
        }
    }

    _Objects.push_back(object);
}

StormSceneObject* StormScene::addNewObject(const std::string& name /* = "" */) {
    StormSceneObject* object = new StormSceneObject(_LastObjectIndex, name);
    _LastObjectIndex++;

    _Objects.push_back(object);
    return object;
}

std::vector<StormSceneObject*>& StormScene::getObjects() {
    return _Objects;
}

StormSceneObject* StormScene::getObjectById(uint32_t id) {
    for (StormSceneObject* object : _Objects) {
        if (object->getId() == id) {
            return object;
        }
    }
    return nullptr;
}

std::vector<SSceneComponentSystem*>& StormScene::getSystems() {
    return _ComponentSystems;
}

void StormScene::render(StormRenderer* renderer) {
    for (unsigned int i = 0; i < _ComponentSystems.size(); i++) {
        _ComponentSystems[i]->render(renderer);
    }
}

void StormScene::tick(float deltaTime) {
    for (unsigned int i = 0; i < _ComponentSystems.size(); i++) {
        _ComponentSystems[i]->tick(deltaTime);
    }
}

void StormScene::initializeDefaultSystems() {
    SSceneSystemTransform* sysTransform = new SSceneSystemTransform();
    _ComponentSystems.push_back(sysTransform);
    _ComponentSystemsByType[S_SCENE_OBJECT_COM_TRANSFORM] = sysTransform;

    SSceneSystemSprite* sysSprite = new SSceneSystemSprite();
    _ComponentSystems.push_back(sysSprite);
    _ComponentSystemsByType[S_SCENE_OBJECT_COM_SPRITE] = sysSprite;

    SSceneSystemPlane* sysPlane = new SSceneSystemPlane();
    _ComponentSystems.push_back(sysPlane);
    _ComponentSystemsByType[S_SCENE_OBJECT_COM_PLANE] = sysPlane;

    SSceneSystemLuaScript* sysLua = new SSceneSystemLuaScript();
    _ComponentSystems.push_back(sysLua);
    _ComponentSystemsByType[S_SCENE_OBJECT_COM_SCRIPT] = sysLua;
}
