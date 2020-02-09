#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QLabel>
#include <OpenSim.h>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //adding the Simulation Tools Widget to the toolBar

    simulationWidget = new SimulationToolsWidget(this);
    ui->simulationToolBar->addWidget(simulationWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_Model_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Model From File"),QString());
    qDebug() << fileName;
    OpenSim::Model  *newModel = new OpenSim::Model(fileName.toStdString());
    qDebug() << QString::fromStdString(newModel->getName());
    navigatorModel = new NavigatorModel(newModel);
    ui->navigatorTreeView->setModel(navigatorModel);
}
