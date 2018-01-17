#include "SEDockObjectHierarchy.h"
#include <QVBoxLayout>
#include <QPushButton>

SEDockObjectHierarchy::SEDockObjectHierarchy(QMainWindow* parent) : SEDockWidget(parent, "Objects Hierarchy") {

    QLayout* layout = SEDockWidget::genericBoxLayout(this);
    _RootWidget->setLayout(layout);

    _ObjectsTree = new SEObjectsTreeWidget(this);
    layout->addWidget(_ObjectsTree);

}

SEDockObjectHierarchy::~SEDockObjectHierarchy() {

}

void SEDockObjectHierarchy::setScene(SScene* scene) {
    _ObjectsTree->populateList(scene);
}