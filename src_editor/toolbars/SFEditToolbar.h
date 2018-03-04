#pragma once
#include "SEToolbar.h"

class SFEditToolbar : public SEToolbar {
    Q_OBJECT

public:
    SFEditToolbar(QWidget* parent);
    virtual ~SFEditToolbar();

public slots:
    /* Callback when clicking on tool icon in toolbar */
    void toolSelected();

private:
    void addTool(const QString& toolName);
};
