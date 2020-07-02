/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020                                                  *
 *                                                                         *
 *   Authors:  Yasser Grimes                                               *
 *   Develeped as a Graduation Project                                     *
 *   School: National School of Computer Science Sidi-Bel-Abbes Algeria    *
 *   Supervisor: Bendaoud Faysal                                           *
 ***************************************************************************/
#include "vsCoordinateDelegate.h"
#include "ui_vsCoordinateDelegate.h"

#include <vsTools/vsOpenSimTools.h>

vsCoordinateDelegate::vsCoordinateDelegate(OpenSim::Coordinate& coordinate,OpenSim::Model *model,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsCoordinateDelegate),
    m_connectedCoordinate(coordinate),
    m_model(model)
{
    ui->setupUi(this);

    ui->nameLabel->setText(QString::fromStdString(coordinate.getName()).left(10));
    ui->nameLabel->setToolTip(QString::fromStdString(coordinate.getName()));
    ui->valueSpinBox->setValue(coordinate.getValue(model->updWorkingState()));
    //ui->speedSpinBox->setValue(coordinate->getSpeedValue(coordinate->getModel().getWorkingState()));
    //ui->lockButton->setChecked(coordinate->getLocked(coordinate->getModel().getWorkingState()));
}

vsCoordinateDelegate::~vsCoordinateDelegate()
{
    delete ui;
}
