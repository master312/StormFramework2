#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSurfaceFormat>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    /*_StormGlWidget = new StormGlWidget;

    QSurfaceFormat glFormat;
    glFormat.setRenderableType(QSurfaceFormat::OpenGL);
    glFormat.setProfile(QS#include <QCoreApplication>urfaceFormat::CoreProfile);
    glFormat.setVersion(3,3);

    _StormGlWidget->setFormat(glFormat);

    this->setCentralWidget(_StormGlWidget);*/
}

MainWindow::~MainWindow()
{
    delete ui;
}
