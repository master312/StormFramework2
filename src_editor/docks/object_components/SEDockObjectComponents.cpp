#include "SEDockObjectComponents.h"
#include "scene/SSceneObject.h"
#include "../../MainWindow.h"
#include "../object_hierarchy/SEDockObjectHierarchy.h"
#include "../../component_widgets/SEComponentWidget.h"
#include <QLayout>
#include <QBoxLayout>
#include <QScrollArea>


SEDockObjectComponents::SEDockObjectComponents(QMainWindow* parent) : SEDockWidget(parent, "Objects Components") {
    QVBoxLayout* vLayout = new QVBoxLayout(_RootWidget);
    QScrollArea* scrollArea = new QScrollArea(_RootWidget);

    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setAlignment(Qt::AlignmentFlag::AlignCenter);
    scrollArea->setWidgetResizable(false);

    QVBoxLayout* scrollAreaVLayout = new QVBoxLayout();
    _ScrollArea = new QWidget();
    _ScrollArea->setLayout(scrollAreaVLayout);
    scrollAreaVLayout->setSizeConstraint(QLayout::SetFixedSize);


    scrollArea->setWidget(_ScrollArea);

    vLayout->addWidget(scrollArea);
    

    connect(MainWindow::get()->getHierarchyDock(), &SEDockObjectHierarchy::sceneObjectSelected,
            this, &SEDockObjectComponents::sceneObjectSelected);
}

SEDockObjectComponents::~SEDockObjectComponents() {

}

void SEDockObjectComponents::sceneObjectSelected(SSceneObject* object) {
    for (int i = 0; i < 10; i++) {
        SEComponentWidget* tmp = new SEComponentWidget(this);


        _ScrollArea->layout()->addWidget(tmp);
        //_RootWidget->layout()->addWidget(tmp);
        //scrollArea->layout()->addWidget(tmp);
    }
}