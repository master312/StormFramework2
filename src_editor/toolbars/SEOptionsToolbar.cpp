#include <src_editor/scene_editing/lua_script/SESystemLuaScript.h>
#include <src_editor/MainWindow.h>
#include "SEOptionsToolbar.h"
#include "docks/object_hierarchy/SEDockObjectHierarchy.h"
#include "scene/SScene.h"

SEOptionsToolbar::SEOptionsToolbar(QWidget* parent) : SEToolbar(parent, "Options") {
    _ToggleGameScripts = addAction("GAME SCRIPTS: off");
    addAction(_ToggleGameScripts);

    connect(_ToggleGameScripts, &QAction::triggered, this, &SEOptionsToolbar::itemClicked);

}

SEOptionsToolbar::~SEOptionsToolbar() {

}

void SEOptionsToolbar::itemClicked() {
    /* TODO: PH code */
    static bool gameScripts = false;
    gameScripts = !gameScripts;

    SScene* scene = MainWindow::getHierarchyDock()->getScene();
    SESystemLuaScript* scriptSystem = static_cast<SESystemLuaScript*>(scene->getScriptSystem());
    if (scriptSystem) {
        scriptSystem->setTickGameScripts(gameScripts);
    }

    if (gameScripts) {
        _ToggleGameScripts->setText("GAME SCRIPTS: on");
    } else {
        _ToggleGameScripts->setText("GAME SCRIPTS: off");
    }
}