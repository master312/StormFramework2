#include "SFEditToolbar.h"
#include "StormEngineEditing.h"

SFEditToolbar::SFEditToolbar(QWidget* parent) : SEToolbar(parent, "Edit") {
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

SFEditToolbar::~SFEditToolbar() {
}

void SFEditToolbar::toolSelected() {
    QObject* toolObj = QObject::sender();
    QString name = toolObj->property("name").toString();

    StormEngineEditing::instanceEditing()->activateTool(name.toLower().toStdString());
}

void SFEditToolbar::addTool(const QString& toolName) {
    QAction* toolMove = addAction(QIcon(QString(":tool_" + toolName).toLower()), toolName);
    toolMove->setProperty("name", QString(toolName));
    addAction(toolMove);

    connect(toolMove, &QAction::triggered, this, &SFEditToolbar::toolSelected);
}

//connect(orderAction, &QAction::triggered, this, &ToolBar::order);
