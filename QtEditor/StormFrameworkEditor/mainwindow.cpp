#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSurfaceFormat>
#include "../../src/StormEngine.h"
#include "../../src/scene/StormScene.h"
#include "../../src/scene/StormSceneObject.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

void MainWindow::on_treeSceneObjects_clicked(const QModelIndex &index) {

}

MainWindow::~MainWindow() {
    delete ui;
}
