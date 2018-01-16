#pragma once
#include <QToolBar>

/* Base class for all editor toolbars */

class SEToolbar : public QToolBar {
    Q_OBJECT
public:
    SEToolbar(QWidget* parent, const QString& name);
    virtual ~SEToolbar();

    static QFont& getToolFont();

public slots:
    void cbViewBarToggle(bool isChecked);
    void cbVisibilityChanged(bool isVisible);

private:
    QAction* _ViewMenuToggle;

    void addToViewMenu();
};
