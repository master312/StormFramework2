#pragma once
#include "../SEDockWidget.h"
#include "SEObjectsTreeWidget.h"

class SEDockObjectHierarchy : public SEDockWidget {
public:
    SEDockObjectHierarchy(QMainWindow* parent);
    virtual ~SEDockObjectHierarchy();

private:
    SEObjectsTreeWidget* _ObjectsTree;
};