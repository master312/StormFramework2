#pragma once
#include "SEToolbar.h"

class SEToolsToolbar : public SEToolbar {
    Q_OBJECT

public:
    SEToolsToolbar(QWidget* parent);
    virtual ~SEToolsToolbar();

public slots:
    /* Callback when clicking on tool icon in toolbar */
    void toolSelected();

private:
    void addTool(const QString& toolName);
};
