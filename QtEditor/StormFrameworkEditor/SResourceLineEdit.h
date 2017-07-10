#pragma once
#include <QLineEdit>

/* Simple widget that opens file browser on double click. */

class SResourceLineEdit : public QLineEdit {
    Q_OBJECT

public:
    SResourceLineEdit(QWidget* parent = nullptr);

signals:
    /* Fires every time when new resource has been selected */
    void resourceSelected();

protected:
    virtual void mousePressEvent(QMouseEvent* event);
};
