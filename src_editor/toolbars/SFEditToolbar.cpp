#include "SFEditToolbar.h"

SFEditToolbar::SFEditToolbar(QWidget* parent) : SEToolbar(parent, "Edit") {
    QAction* undoButton = addAction(QIcon::fromTheme("edit-undo", QIcon(":/edit-undo.png")), "Undo");
    addAction(undoButton);

    QAction* redoButton = addAction(QIcon::fromTheme("edit-redo", QIcon(":/edit-redo.png")), "Redo");
    addAction(redoButton);

    addSeparator();

    QAction* toolMove = addAction(QIcon(":tool_move"), "Move");
    addAction(toolMove);

    QAction* toolResize = addAction(QIcon(":tool_resize"), "Resize");
    addAction(toolResize);

    QAction* toolRotate = addAction(QIcon(":tool_rotate"), "Rotate");
    addAction(toolRotate);
}

SFEditToolbar::~SFEditToolbar() {
}