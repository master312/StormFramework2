#include "selectprefab.h"
#include "ui_selectprefab.h"
#include <QPushButton>
#include <QtWidgets/QScrollArea>
#include "../src/StormEngine.h"
#include "../src/scene/StormScene.h"

SelectPrefab::SelectPrefab(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectPrefab)
{
    ui->setupUi(this);

    if (!StormEngine::getActiveScene()) {
        return;
    }

    StormScene* scene = StormEngine::getActiveScene();

    QVBoxLayout* listArea = ui->prefabsList;
    for (auto& iter : scene->getPrefabs()) {
        QPushButton* btn = new QPushButton(this);
        btn->setText(iter.first.c_str());
        connect(btn, SIGNAL(clicked()), this, SLOT(prefabSelectedToInstantiate()));
        listArea->addWidget(btn);
    }

}

SelectPrefab::~SelectPrefab() {
    delete ui;
}

void SelectPrefab::prefabSelectedToInstantiate() {
    QPushButton* obj = dynamic_cast<QPushButton*>(sender());
    if (!obj) {
        return;
    }

    StormScene* scene = StormEngine::getActiveScene();
    if (!scene) {
        return;
    }

    scene->instantiatePrefab(obj->text().toStdString(), "Instantiated_Prefab");
}
