#pragma once
#include <QToolBar>
#include <QMenu>

class SEFileToolbar : public QToolBar {
    Q_OBJECT
public:
    SEFileToolbar(QWidget* parent);
    virtual ~SEFileToolbar();
};
