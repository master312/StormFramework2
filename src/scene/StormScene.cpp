#include "StormScene.h"
#include "StormSceneObject.h"
#include "components/SSceneComPlane.h"
#include "components/SSceneComStaticTexture.h"
#include "components/SSceneSystemStaticTexture.h"

#include "../StormEngine.h"
#include "../StormEngineEditing.h"
#include "../core/graphics/StormRenderer.h"
#include "../core/resources/StormFileSystem.h"

StormScene::StormScene() {
    _LastObjectIndex = 1;
    _Name = "";

    initializeDefaultSystems();

#ifdef _EDITING_SUPPORT
    initEditing();
#endif
}

StormScene::~StormScene() {
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

    for (pugi::xml_node objectNode = sceneRootNode.first_child(); objectNode; objectNode = objectNode.next_sibling()) {
        StormSceneObject* object = new StormSceneObject();
        if (object->deserializeXml(objectNode) < 0) {
            LOG(ERROR) << "Object XML deserialization error";
            continue;
        }

        addObject(object);
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

void StormScene::render(StormRenderer* renderer) {
    for (unsigned int i = 0; i < _ComponentSystems.size(); i++) {
        _ComponentSystems[i]->render(renderer);
    }

#ifdef _EDITING_SUPPORT
    renderEditing(renderer);
#endif
}

void StormScene::tick(float deltaTime) {
    for (unsigned int i = 0; i < _ComponentSystems.size(); i++) {
        _ComponentSystems[i]->tick(deltaTime);
    }
}

void StormScene::initializeDefaultSystems() {

    SSceneSystemStaticTexture* sysTexture = new SSceneSystemStaticTexture();
    _ComponentSystems.push_back(sysTexture);
    _ComponentSystemsByType[S_SCENE_OBJECT_COM_TEXTURE] = sysTexture;
}

#ifdef _EDITING_SUPPORT

void StormScene::initEditing() {
    _ObjectSelectedForEdit = nullptr;
}

void StormScene::renderEditing(StormRenderer* renderer) {
    if (!StormEngineEditing::isEditMode()) {
        return;
    }
    ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(600, 600));
    ImGui::Begin("Scene editing window", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Scene name: %s", _Name.c_str());
    
    if (ImGui::Button("Save scene file")) {
        saveXml();
    }

    if (ImGui::TreeNode("Scene object hierarchy")) {
        renderObjectHierarchyGui();
        ImGui::TreePop();
    }
    
    ImGui::Separator();

    if (_ObjectSelectedForEdit) {
        ImGui::Text("Editing object %s ID:%d", _ObjectSelectedForEdit->getName().c_str(), _ObjectSelectedForEdit->getId());
        renderObjectEditingGui(_ObjectSelectedForEdit);
    } else {
        ImGui::Text("No object selected");
    }

    ImGui::End();
}

void StormScene::renderObjectHierarchyGui() {
    for (unsigned int i = 0; i < _Objects.size(); i++) {
        StormSceneObject* obj = _Objects[i];
        if (ImGui::TreeNode((void*)(intptr_t)i, "ID:%d %s", obj->getId(), obj->getName().c_str())) {
            // TODO: Render childen here
            ImGui::TreePop();        
        } else {
            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)) {
                _ObjectSelectedForEdit = obj;
            }
        }
        if (ImGui::IsItemHovered() && _ObjectSelectedForEdit != obj) {
            ImGui::SetTooltip("Right click to edit");
        }
    }
}

void StormScene::renderObjectEditingGui(StormSceneObject* object) {
    //ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(600, 600));
    //ImGui::Begin("Editing object", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    std::stringstream ss;
    for (SSceneComponent* com : object->getComponents()) {
        ss.str("");
        ss << SSceneComponentTypeString[com->getType()] << " component";
        if (ImGui::CollapsingHeader(ss.str().c_str())) {
            com->renderEditingGui();
        }
    }

   // ImGui::End();
}

#endif