#include "SEDockDebugSettings.h"
#include "StormCommon.h"
#include "StormDebug.h"
#include <QLayout>
#include <QLabel>
#include <QCheckBox>

SEDockDebugSettings::SEDockDebugSettings(QMainWindow* parent) : SEDockWidget(parent, "Debug Settings") {
    QLayout* layout = SEDockWidget::genericBoxLayout(_RootWidget);

    QLabel* tickingSystems = new QLabel("Toggle systems logic:", this);
    layout->addWidget(tickingSystems);

    /* Disable physics ticking by default */
    StormDebug::setSystemTickingEnabled(S_SCENE_OBJECT_COM_PHYSICS, false);

    generateSystemTickingCheckBoxes(layout);
}

SEDockDebugSettings::~SEDockDebugSettings() {
}

void SEDockDebugSettings::systemTickChanged(int value) {
    QObject* senderObj = QObject::sender();
    QWidget* widget = dynamic_cast<QWidget*>(senderObj);
    if (!senderObj || !widget) {
        LOG(WARNING) << "Signal SEDockDebugSettings::systemTickChanged received without sender object";
        return;
    }
    QVariant property = widget->property("storm_sys_type");
    int systemType = property.toInt();
    if (systemType <= 0) {
        return;
    }
    StormDebug::setSystemTickingEnabled((SSceneComponentType)systemType, value > 0);
    if (value <= 0) {
        LOG(INFO) << "System " << SSceneComponentTypeString[systemType] << " ticking disabled";
    } else {
        LOG(INFO) << "System " << SSceneComponentTypeString[systemType] << " ticking enabled";
    }
}

void SEDockDebugSettings::generateSystemTickingCheckBoxes(QLayout* layout) {
    for (int i = 0; i < SSceneComponentTickingOrderCount; i++) {
        int systemType = SSceneComponentTickingOrder[i];
        if (systemType <= 0) {
            continue;
        }
        bool shouldSystemTick = StormDebug::shouldTickSystem((SSceneComponentType)systemType);
        std::string systemName = SSceneComponentTypeString[systemType];

        QCheckBox* checkBox = new QCheckBox(systemName.c_str(), this);
        checkBox->setProperty("storm_sys_type", systemType);
        checkBox->setChecked(shouldSystemTick);

        connect(checkBox, &QCheckBox::stateChanged, this, &SEDockDebugSettings::systemTickChanged);

        layout->addWidget(checkBox);
    }
}