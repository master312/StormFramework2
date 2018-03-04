#include "SEToolsToolbar.h"
#include "StormEngineEditing.h"

SEToolsToolbar::SEToolsToolbar(QWidget* parent) : SEToolbar(parent, "Tools") {
    QAction* undoButton = addAction(QIcon::fromTheme("edit-undo", QIcon(":/edit-undo.png")), "Undo");
    addAction(undoButton);

    QAction* redoButton = addAction(QIcon::fromTheme("edit-redo", QIcon(":/edit-redo.png")), "Redo");
    addAction(redoButton);

    addSeparator();

    /* TODO: Load this from script */
    addTool("Position");
    addTool("Resize");
    addTool("Rotate");
}

SEToolsToolbar::~SEToolsToolbar() {
}

void SEToolsToolbar::toolSelected() {
    QObject* toolObj = QObject::sender();
    QString name = toolObj->property("name").toString();

    StormEngineEditing::instanceEditing()->activateTool(name.toLower().toStdString());
}

void SEToolsToolbar::addTool(const QString& toolName) {
    QAction* toolMove = addAction(QIcon(QString(":tool_" + toolName).toLower()), toolName);
    toolMove->setProperty("name", QString(toolName));
    addAction(toolMove);

    connect(toolMove, &QAction::triggered, this, &SEToolsToolbar::toolSelected);
}

//connect(orderAction, &QAction::triggered, this, &ToolBar::order);
