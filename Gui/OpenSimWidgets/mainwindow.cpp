#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QLabel>

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

