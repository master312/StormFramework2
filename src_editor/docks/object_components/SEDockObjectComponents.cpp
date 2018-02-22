#include "SEDockObjectComponents.h"
#include "scene/SSceneObject.h"
#include "utils/math/Vector2.h"
#include "../../MainWindow.h"
#include "../object_hierarchy/SEDockObjectHierarchy.h"
#include "component_widgets/SERootComponentWidget.h"
#include "property_widgets/SEPropertyVector2.h"
#include <QScrollArea>


SEDockObjectComponents::SEDockObjectComponents(QMainWindow* parent) : SEDockWidget(parent, "Objects Components") {
    QLayout* vLayout = SEDockWidget::genericBoxLayout(_RootWidget);
    QScrollArea* scrollArea = new QScrollArea(_RootWidget);

    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setAlignment(Qt::AlignmentFlag::AlignCenter);
    scrollArea->setWidgetResizable(false);

    QVBoxLayout* scrollAreaVLayout = new QVBoxLayout();
    scrollAreaVLayout->setSizeConstraint(QLayout::SetFixedSize);
    scrollAreaVLayout->setSpacing(1);
    scrollAreaVLayout->setMargin(0);
    scrollAreaVLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    _ScrollArea = new QWidget(this);
    _ScrollArea->setLayout(scrollAreaVLayout);

    scrollArea->setWidget(_ScrollArea);

    vLayout->addWidget(scrollArea);

    connect(MainWindow::getHierarchyDock(), &SEDockObjectHierarchy::sceneObjectSelected,
            this, &SEDockObjectComponents::sceneObjectSelected);
}

SEDockObjectComponents::~SEDockObjectComponents() {

}

void SEDockObjectComponents::sceneObjectSelected(SSceneObject* object) {
    for (int i = 0; i < 10; i++) {
        SERootComponentWidget* tmp = new SERootComponentWidget(this);


        SEPropertyVector2* wdg = new SEPropertyVector2(tmp, "THE TIELE!");
        tmp->addPropertyWidget(wdg);


        _ScrollArea->layout()->addWidget(tmp);
        //_RootWidget->layout()->addWidget(tmp);
        //scrollArea->layout()->addWidget(tmp);
    }
}