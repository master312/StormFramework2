#include "SEDockObjectHierarchy.h"
#include <QVBoxLayout>
#include <QPushButton>

SEDockObjectHierarchy::SEDockObjectHierarchy(QMainWindow* parent) : SEDockWidget(parent, "Objects Hierarchy") {

    QLayout* layout = SEDockWidget::genericBoxLayout(this);
    _RootWidget->setLayout(layout);

    _ObjectsTree = new SEObjectsTreeWidget(this);
    layout->addWidget(_ObjectsTree);


//    QWidget *newWidget = new QWidget(this);
//
//
//
//    QVBoxLayout* layout = new QVBoxLayout();
//    layout->setSpacing(1);
//    layout->setMargin(0);
//    layout->setAlignment(Qt::AlignmentFlag::AlignTop);
//
//    _ObjectsTree = new SEObjectsTreeWidget(this);
//    layout->addWidget(_ObjectsTree);
//
//    QPushButton* _DestroyButtonButton = new QPushButton(this);
//    _DestroyButtonButton->setText("X");
//    layout->addWidget(_DestroyButtonButton);
//    _DestroyButtonButton = new QPushButton(this);
//    _DestroyButtonButton->setText("y");
//   // _DestroyButtonButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
//
//    layout->addWidget(_DestroyButtonButton);
//
//    newWidget->setLayout(layout);
//
//    setWidget(newWidget);
}

SEDockObjectHierarchy::~SEDockObjectHierarchy() {

}
