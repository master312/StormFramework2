#include "SEMainEditorDock.h"
#include "SEStormMainWidget.h"
#include <QLayout>

SEMainEditorDock::SEMainEditorDock(QMainWindow* parent) : SEDockWidget(parent, "Editor Main") {
    QLayout* layout = SEMainEditorDock::genericBoxLayout(this);
    _RootWidget->setLayout(layout);

    setMinimumSize(500, 500);

    setFeatures(DockWidgetFloatable);

    setAllowedAreas(Qt::NoDockWidgetArea);

    _MainWidget = new SEStormMainWidget(this);
    _MainWidget->setMinimumSize(500, 500);
    layout->addWidget(_MainWidget);

    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
}

SEMainEditorDock::~SEMainEditorDock() {
}