#pragma once
#include "../SEDockWidget.h"
#include "SEObjectsTreeWidget.h"

class SScene;

class SEDockObjectHierarchy : public SEDockWidget {
public:
    SEDockObjectHierarchy(QMainWindow* parent);
    virtual ~SEDockObjectHierarchy();

    void setScene(SScene* scene);

private:
    SEObjectsTreeWidget* _ObjectsTree;
};