#pragma once
#include <QTreeWidget>

class SStormScene;

class SEObjectsTreeWidget : public QTreeWidget {
    Q_OBJECT
public:
    SEObjectsTreeWidget(QWidget* parent);
    virtual ~SEObjectsTreeWidget();

    /* Populate list with scene objects hierarchy from @scene */
    void populateList(SStormScene* scene);
};