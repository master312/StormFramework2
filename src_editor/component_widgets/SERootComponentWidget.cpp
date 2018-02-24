#include "SERootComponentWidget.h"
#include "MainWindow.h"
#include "docks/object_hierarchy/SEDockObjectHierarchy.h"
#include "scene/SScene.h"
#include "scene_editing/lua_script/SESystemLuaScript.h"

SERootComponentWidget::SERootComponentWidget(QWidget* parent) : QWidget(parent) {
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    _Component = nullptr;

    _Layout = new QVBoxLayout();
    _Layout->setSpacing(1);
    _Layout->setMargin(0);
    _Layout->setAlignment(Qt::AlignmentFlag::AlignLeft | Qt::AlignmentFlag::AlignTop);

    setMaximumSize(6000, 6000);
    setMinimumSize(180, 20);

    _ToggleButton = new QPushButton(this);
    _ToggleButton->setText("Ovo je sve matrix!");
    _ToggleButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    connect(_ToggleButton, SIGNAL(clicked(bool)), this, SLOT(toggleCollapse()));
    _Layout->addWidget(_ToggleButton);

    setLayout(_Layout);

    /* Set black background */
//    QPalette pallete = palette();
//    pallete.setColor(QPalette::Background, Qt::black);
//    setAutoFillBackground(true);
//    setPalette(pallete);
}

SERootComponentWidget::~SERootComponentWidget() {
}

int SERootComponentWidget::loadComponent(SSceneComponent* component) {
    SScene* scene = MainWindow::getHierarchyDock()->getScene();
    if (!scene) {
        LOG(ERROR) << "SERootComponentWidget::loadComponent No scene found!";
        return -1;
    }

    std::string filename = "lua_editor/component_widgets/";
    filename += SSceneComponentTypeString[(int)component->getType()];
    filename += ".lua";
    std::transform(filename.begin(), filename.end(), filename.begin(), ::tolower);

    SESystemLuaScript* scriptSystem = static_cast<SESystemLuaScript*>(scene->getScriptSystem());
    sol::table table = scriptSystem->loadScriptFile(filename);
    if (!table.valid()) {
        return -2;
    }

    /* Create lua handle by calling 'createComponentWidgetHandle' lua function */
    sol::function createHandle = scriptSystem->getLua()["createComponentWidgetHandle"];
    if (createHandle && createHandle.valid()) {
        _LuaHandle = createHandle(this, table);
        LOG(INFO) << (int)_LuaHandle["index"];
        if (!_LuaHandle.valid()) {
            LOG(ERROR) << "Could not create lua handle for component widget type: " << component->getType();
            return -3;
        }
    } else {
        LOG(ERROR) << "Lua function 'createComponentWidgetHandle' not found";
        return -4;
    }

    LOG(DEBUG) << "Component widget script '" << filename << "' loaded";
    return 1;
}

int SERootComponentWidget::initialize() {
    sol::function function = _LuaHandle["script"]["onCreated"];
    if (function.valid()) {
        function();
    } else {
        LOG(ERROR) << "Could not find 'onCreated' lua method for component widget.";
        return -1;
    }
    return 1;
}

void SERootComponentWidget::toggleCollapse() {
    /* Collapse (header) button click callback */
    foreach (QWidget* child, findChildren<QWidget*>()) {
        if (child != _ToggleButton) {
            child->setHidden(true);
        }
    }
}

void SERootComponentWidget::addPropertyWidget(SEPropertyWidget* widget) {
    widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    _Layout->addWidget(widget);
}