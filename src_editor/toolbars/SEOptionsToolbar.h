#pragma once
#include "SEToolbar.h"

/*
 *
 * TODO: ** WARNING **
 * PLACE HOLDER CODE
 * Rewrite the whole thing when next extension is needed
 *
 *
 *
 *
 *
 */

class SEOptionsToolbar : public SEToolbar {
    Q_OBJECT

public:
    SEOptionsToolbar(QWidget* parent);
    virtual ~SEOptionsToolbar();

public slots:
    void itemClicked();

private:
    QAction* _ToggleGameScripts;
};
