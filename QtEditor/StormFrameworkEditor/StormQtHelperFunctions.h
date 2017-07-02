#pragma once
#include <iostream>
#include <QWidget>

class StormQtHelper {
public:

    /* Recursive method that iterates trough all childrend and children of children,
     * until widget with @name is found. */
    static QWidget* findChildByName(QWidget* parent, const std::string& name, bool checkParent = true) {
        if (!parent) {
            return nullptr;
        }
        if (parent->parentWidget() && checkParent) {
            return findChildByName(parent->parentWidget(), name);
        }
        QList<QWidget*> widgets = parent->findChildren<QWidget*>();
        foreach (QWidget* child, widgets) {
            if (child->objectName().compare(name.c_str()) == 0) {
                return child;
            } else {
                QWidget* tmp = findChildByName(child, name, false);
                if (tmp) {
                    return tmp;
                }
            }
        }
        return nullptr;
    }
};
