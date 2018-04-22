#include "SEDockObjectHierarchy.h"
#include <QVBoxLayout>
#include "scene/SSceneManager.h"
#include "scene_editing/lua_script/SESystemLuaScript.h"
#include "StormEngine.h"

SEDockObjectHierarchy::SEDockObjectHierarchy(QMainWindow* parent) : SEDockWidget(parent, "Objects Hierarchy") {

    QLayout* layout = SEDockWidget::genericBoxLayout();
    _RootWidget->setLayout(layout);

    _ObjectsTree = new SEObjectsTreeWidget(this);
    layout->addWidget(_ObjectsTree);

    _SelectedObject = nullptr;

    StormEngine::getEventDispatcher()->registerEventListener<SEDockObjectHierarchy>(
            SEventDispatcher::SceneChangeEvent::ABOUT_TO_CHANGE,
            &SEDockObjectHierarchy::cbSceneAboutToChange, this);
}

SEDockObjectHierarchy::~SEDockObjectHierarchy() {
    StormEngine::getEventDispatcher()->removeListeners<SEDockObjectHierarchy>(this);
}

void SEDockObjectHierarchy::cbObjectSelected(SSceneObject* selectedObject) {
    if (_SelectedObject == selectedObject) {
        return;
    }
    _SelectedObject = selectedObject;

    SEventDispatcher::SSceneObjectEvent event(_SelectedObject, SEventDispatcher::SSceneObjectEvent::EDIT_OBJECT_SELECTED);
    StormEngine::fireEvent(&event);
}

void SEDockObjectHierarchy::setScene(SScene* scene) {
    _ObjectsTree->populateList(scene);
    _Scene = scene;
}

SScene* SEDockObjectHierarchy::getScene() {
    return _Scene;
}

SSceneObject* SEDockObjectHierarchy::getSelectedObject() {
    return _SelectedObject;
}

void SEDockObjectHierarchy::cbSceneAboutToChange(const SEventDispatcher::Event* event) {
    const SEventDispatcher::SceneChangeEvent* scEvent = static_cast<const SEventDispatcher::SceneChangeEvent*>(event);
    if (!scEvent || _Scene == scEvent->scene) {
        return;
    }
    /* Deselect object */
    cbObjectSelected(nullptr);

    /* Destroy scene and hierarchy tree items */
    _Scene = nullptr;
    _ObjectsTree->clearList();
    if (!scEvent->scene) {
        LOG(WARNING) << "Scene is about to change, but next scene is nullptr";
        return;
    }
    setScene(scEvent->scene);
}