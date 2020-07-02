/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsCoordinateDelegate.h"
#include "ui_vsCoordinateDelegate.h"

#include <vsTools/vsOpenSimTools.h>

#define _COORDINATE m_model->updCoordinateSet().get(m_coordinateIndex)

vsCoordinateDelegate::vsCoordinateDelegate(int coordinate_index/*OpenSim::Coordinate& coordinate*/,OpenSim::Model *model,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsCoordinateDelegate),
    m_connectedCoordinate(nullptr),
    m_model(model),
    m_coordinateIndex(coordinate_index)
{
    ui->setupUi(this);

    ui->nameLabel->setText(QString::fromStdString(_COORDINATE.getName()).left(10));
    ui->nameLabel->setToolTip(QString::fromStdString(_COORDINATE.getName()));
    ui->valueSpinBox->setValue(_COORDINATE.getValue(model->updWorkingState()));
    ui->speedSpinBox->setValue(_COORDINATE.getSpeedValue(m_model->getWorkingState()));
    ui->lockButton->setChecked(_COORDINATE.getLocked(m_model->getWorkingState()));
}

vsCoordinateDelegate::~vsCoordinateDelegate()
{
    delete ui;
}
