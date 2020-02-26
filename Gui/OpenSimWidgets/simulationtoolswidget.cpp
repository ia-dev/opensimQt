#include "simulationtoolswidget.h"
#include "ui_simulationtoolswidget.h"
#include <QDebug>
#include <QDragEnterEvent>

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

void SimulationToolsWidget::dragEnterEvent(QDragEnterEvent *event)
{

}
