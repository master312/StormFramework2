#include "SEFileToolbar.h"
#include "SEToolbarsCommon.h"

SEFileToolbar::SEFileToolbar(QWidget* parent) : QToolBar(parent) {
    setIconSize(QSize(28, 28));
    /* This style shows text under icon in menu */
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    setObjectName("File Toolbar");
    setWindowTitle("File Toolbar");

    QAction* newScene = addAction(QIcon::fromTheme("document-new", QIcon(":/doc-new.png")), "New");
    newScene->setFont(SEToolbarsCommon::getToolFont());
    addAction(newScene);

    QAction* openScene = addAction(QIcon::fromTheme("document-open", QIcon(":/doc-open.png")), "Open");
    openScene->setFont(SEToolbarsCommon::getToolFont());
    addAction(openScene);

    QAction* saveScene = addAction(QIcon::fromTheme("document-save", QIcon(":/doc-save.png")), "Save");
    saveScene->setFont(SEToolbarsCommon::getToolFont());
    addAction(saveScene);
}

SEFileToolbar::~SEFileToolbar() {

}