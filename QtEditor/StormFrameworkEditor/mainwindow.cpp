#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "selectprefab.h"
#include <QSurfaceFormat>
#include <QFileDialog>
#include "../../src/StormEngine.h"
#include "../../src/StormDebug.h"
#include "../../src/scene/StormScene.h"
#include "../../src/scene/StormSceneObject.h"
#include "../../src/core/resources/StormFileSystem.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

void MainWindow::on_treeSceneObjects_clicked(const QModelIndex &index) {
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    /* Toggle physics */
    StormDebug* dbg = StormDebug::instance();
    dbg ->setShouldTickPhysics(!dbg->shouldTickPhysics());
}

void MainWindow::on_pushButton_2_clicked() {
    /* Toggle logic */
    StormDebug* dbg = StormDebug::instance();
    dbg ->setShouldTickLogic(!dbg->shouldTickLogic());
}

void MainWindow::on_btn_InstantiatePrefab_clicked() {
    SelectPrefab* selectPrefab = new SelectPrefab(this);
    selectPrefab->show();
}

void MainWindow::on_btn_Save_clicked() {
    std::string path = StormEngine::getModule<StormFileSystem>()->getRootPath();

    QString filename = QFileDialog::getSaveFileName(this,
                            tr("Save scene file"),
                            path.c_str(), tr("XML Files (*.xml)"));

    if (StormEngine::getActiveScene()) {
        StormEngine::getActiveScene()->saveXml(filename.toStdString());
    }

}
