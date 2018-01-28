#include "SEDockObjectComponents.h"
#include "scene/SSceneObject.h"
#include "../../MainWindow.h"
#include "../object_hierarchy/SEDockObjectHierarchy.h"

SEDockObjectComponents::SEDockObjectComponents(QMainWindow* parent) : SEDockWidget(parent, "Objects Components") {
    QLayout* layout = SEDockWidget::genericBoxLayout(this);
    _RootWidget->setLayout(layout);


    connect(MainWindow::get()->getHierarchyDock(), &SEDockObjectHierarchy::sceneObjectSelected,
            this, &SEDockObjectComponents::sceneObjectSelected);
}

SEDockObjectComponents::~SEDockObjectComponents() {

}

void SEDockObjectComponents::sceneObjectSelected(SSceneObject* object) {

}