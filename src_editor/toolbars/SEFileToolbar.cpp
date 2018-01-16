#include "SEFileToolbar.h"

SEFileToolbar::SEFileToolbar(QWidget* parent) : SEToolbar(parent, "File") {
    /* This style shows text under icon in menu */
    // setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    QAction* newScene = addAction(QIcon::fromTheme("document-new", QIcon(":/doc-new.png")), "New");
    addAction(newScene);

    QAction* openScene = addAction(QIcon::fromTheme("document-open", QIcon(":/doc-open.png")), "Open");
    addAction(openScene);

    QAction* saveScene = addAction(QIcon::fromTheme("document-save", QIcon(":/doc-save.png")), "Save");
    addAction(saveScene);

}

SEFileToolbar::~SEFileToolbar() {

}