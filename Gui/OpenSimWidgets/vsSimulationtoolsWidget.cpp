#include "vsSimulationToolsWidget.h"
#include "ui_vsSimulationToolsWidget.h"
#include <QDebug>
#include <QDragEnterEvent>

vsSimulationToolsWidget::vsSimulationToolsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsSimulationToolsWidget)
{
    ui->setupUi(this);

}

vsSimulationToolsWidget::~vsSimulationToolsWidget()
{
    delete ui;
}

void vsSimulationToolsWidget::dragEnterEvent(QDragEnterEvent *event)
{

}
