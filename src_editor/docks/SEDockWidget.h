#pragma once
#include <QDockWidget>
#include <QMenu>
#include <QString>

class SEDockWidget : public QDockWidget {
    Q_OBJECT
public:
    explicit SEDockWidget(QMainWindow* parent, const QString& name);

public slots:
    void cbViewBarToggle(bool isChecked);
    void cbVisibilityChanged(bool isVisible);

protected:
    QWidget* _RootWidget;


    static QLayout* genericBoxLayout(SEDockWidget* parent);

private:
    QAction* _ViewMenuToggle;

    /* Adds toggle option for this widget to View menu */
    void addToViewMenu();

};
