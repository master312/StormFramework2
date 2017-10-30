#include "SResourceLineEdit.h"
#include "../../src/StormEngine.h"
#include "../../src/core/resources/StormFileSystem.h"
#include <QFileDialog>

SResourceLineEdit::SResourceLineEdit(QWidget* parent) : QLineEdit(parent) {
    setReadOnly(true);
    setFocusPolicy(Qt::FocusPolicy::NoFocus);
}

void SResourceLineEdit::mousePressEvent(QMouseEvent* event) {
    std::string rootPath = QDir::currentPath().toStdString();
    rootPath += "/" + StormEngine::getModule<StormFileSystem>()->getRootPath();

    QString filename = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    QString(rootPath.c_str()));
    if (!filename.size()) {
        /* No file selected */
        return;
    }
    filename.remove(0, rootPath.size());

    if (text() != filename) {
        setText(filename);
        resourceSelected();
    }
}
