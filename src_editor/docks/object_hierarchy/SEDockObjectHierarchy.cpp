#include "SEDockObjectHierarchy.h"
#include <QVBoxLayout>
#include <QPushButton>
#include "StormEngine.h"

SEDockObjectHierarchy::SEDockObjectHierarchy(QMainWindow* parent) : SEDockWidget(parent, "Objects Hierarchy") {

    QLayout* layout = SEDockWidget::genericBoxLayout(this);
    _RootWidget->setLayout(layout);

    _ObjectsTree = new SEObjectsTreeWidget(this);
    layout->addWidget(_ObjectsTree);
}

SEDockObjectHierarchy::~SEDockObjectHierarchy() {
    S_REMOVE_GLOBAL_NOTIFICATION_LISTENER(_ObjectsTree);
}

void SEDockObjectHierarchy::setScene(SScene* scene) {
    S_ADD_GLOBAL_NOTIFICATION_LISTENER(SNotificationType::SCENE_OBJECT_ADDED, _ObjectsTree, SEObjectsTreeWidget::newPrefabInstantiated);
    _ObjectsTree->populateList(scene);
}