#include "SESceneObjectTreeItem.h"
#include "scene/SSceneObject.h"

SESceneObjectTreeItem::SESceneObjectTreeItem(QTreeWidget* parent, SSceneObject* object) : QTreeWidgetItem(parent) {
    _SceneObject = object;
    std::string name = object->getName();
    QFont curFont = font(0);
    if (name != "") {
        setText(0, name.c_str());
    } else {
        setText(0, "Object ID:" + QString::number(object->getId()));
        curFont.setItalic(true);
        setFont(0, curFont);
    }

    setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
}

SESceneObjectTreeItem::~SESceneObjectTreeItem() {
}

SSceneObject* SESceneObjectTreeItem::getSceneObject() {
    return _SceneObject;
}