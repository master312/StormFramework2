#include "StormScene.h"
#include "StormSceneObject.h"

#include "components/SSceneSystemPlane.h"
#include "components/SSceneSystemStaticTexture.h"
#include "components/SSceneSystemTransform.h"

#include "../StormEngine.h"
#include "../core/graphics/StormRenderer.h"
#include "../core/resources/StormFileSystem.h"

StormScene::StormScene() {
    _LastObjectIndex = 1;
    _Name = "";

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
    _ComponentSystemsByType.clear();
}

int StormScene::loadXml(spStormResourceFile xmlFile) {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load(xmlFile->getBuffer(), xmlFile->getBufferSize());
    if (result.status != pugi::status_ok) {
        LOG(ERROR) << "Scene XML " << xmlFile->getFilename() << " error: " << result.description();
        return -1;
    }
    
    pugi::xml_node sceneRootNode = doc.child("scene");
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

    if (StormEngine::instance()->getDataFilesystem()->saveResourceFile(file) > 0) {
        LOG(INFO) << "Scene '" << _Name << "' saved to xml";
    } else {
        LOG(DEBUG) << "Error occured while saving scene'" << _Name << "'";
    }
}

void StormScene::initialize() {
    for (SSceneComponentSystem* componentSystem : _ComponentSystems) {
        componentSystem->initialize();
    }
    LOG(INFO) << "Scene '" << _Name << "' component systems initialized";
}

void StormScene::setName(const std::string& name) {
    _Name = name;
}

std::string StormScene::getName() const {
    return _Name;
}

void StormScene::addObject(StormSceneObject* object) {
    for (SSceneComponent* com : object->getComponents()) {
        auto iter = _ComponentSystemsByType.find(com->getType());
        if (iter != _ComponentSystemsByType.end()) {
            iter->second->addComponent(com);
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

void StormScene::render(StormRenderer* renderer) {
    for (unsigned int i = 0; i < _ComponentSystems.size(); i++) {
        _ComponentSystems[i]->render(renderer);
    }
}
#include "components/SSceneComTransform.h"
void StormScene::tick(float deltaTime) {
    SSceneComTransform* com = dynamic_cast<SSceneComTransform*>(_Objects[0]->getComponent(S_SCENE_OBJECT_COM_TRANSFORM));
    if (com) {
        com->setAngle(com->getAngle() + 2.5f);
    }
    for (unsigned int i = 0; i < _ComponentSystems.size(); i++) {
        _ComponentSystems[i]->tick(deltaTime);
    }
}

void StormScene::initializeDefaultSystems() {
    SSceneSystemTransform* sysTransform = new SSceneSystemTransform();
    _ComponentSystems.push_back(sysTransform);
    _ComponentSystemsByType[S_SCENE_OBJECT_COM_TRANSFORM] = sysTransform;

    SSceneSystemStaticTexture* sysTexture = new SSceneSystemStaticTexture();
    _ComponentSystems.push_back(sysTexture);
    _ComponentSystemsByType[S_SCENE_OBJECT_COM_STATIC_TEXTURE] = sysTexture;

    SSceneSystemPlane* sysPlane = new SSceneSystemPlane();
    _ComponentSystems.push_back(sysPlane);
    _ComponentSystemsByType[S_SCENE_OBJECT_COM_PLANE] = sysPlane;
}
