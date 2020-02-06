#include "simulationtoolswidget.h"
#include "ui_simulationtoolswidget.h"

SimulationToolsWidget::SimulationToolsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SimulationToolsWidget)
{
    ui->setupUi(this);

}

SimulationToolsWidget::~SimulationToolsWidget()
{
    delete ui;
}
