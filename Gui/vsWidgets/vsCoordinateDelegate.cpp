/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Yasser Grimes                            *
 *                                                                         *
 ***************************************************************************/
#include "vsCoordinateDelegate.h"
#include "ui_vsCoordinateDelegate.h"
#include "vsCoordinatesWidget.h"

#include <vsTools/vsOpenSimTools.h>

#include <vsModeling/vsNavigatorNode.h>
#include <QState>
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
    ui->valueSpinBox->setValue(SimTK::convertRadiansToDegrees(_COORDINATE.getValue(model->updWorkingState())));
    ui->speedSpinBox->setValue(_COORDINATE.getSpeedValue(m_model->getWorkingState()));
    ui->seekSlider->setValue(ui->valueSpinBox->value());
    ui->seekSlider->setMinimum(SimTK::convertRadiansToDegrees(_COORDINATE.getRangeMin()));
    ui->seekSlider->setMaximum(SimTK::convertRadiansToDegrees(_COORDINATE.getRangeMax()));
    ui->lockButton->setChecked(_COORDINATE.getLocked(m_model->getWorkingState()));

    ui->boundMin->setText(QString::number(ui->seekSlider->minimum()));
    ui->boundMax->setText(QString::number(ui->seekSlider->maximum()));
    m_isLoading = false;
}

vsCoordinateDelegate::~vsCoordinateDelegate()
{
    delete ui;
}

void vsCoordinateDelegate::updateModelGeometries()
{
    if(m_isLoading) return;
    m_model->realizePosition(m_model->updWorkingState());
    vsNavigatorNode::visualizerVTK->updating = true;
    vsNavigatorNode::visualizerVTK->updateModelDecorations(m_model);

}

CoordinateInfo vsCoordinateDelegate::getDefaultCoordinateInfo()
{
    return CoordinateInfo(_COORDINATE.getDefaultValue(),_COORDINATE.getDefaultSpeedValue(),
                          _COORDINATE.getDefaultLocked(),_COORDINATE.getDefaultClamped());
}

void vsCoordinateDelegate::setCoordinateInfo(CoordinateInfo info)
{
    ui->valueSpinBox->setValue(info.m_value);
    ui->seekSlider->setValue(info.m_value);
    ui->speedSpinBox->setValue(info.m_value);
    ui->lockButton->setChecked(info.m_isLocked);
    ui->clampButton->setChecked(info.m_isClamped);
}

CoordinateInfo vsCoordinateDelegate::getCurrentCoordinateInfo()
{
    return CoordinateInfo(ui->valueSpinBox->value(),ui->speedSpinBox->value(),
                          ui->lockButton->isChecked(),ui->clampButton->isChecked());

}

void vsCoordinateDelegate::on_valueSpinBox_valueChanged(double arg1)
{
    if(m_isLoading)return;
    if(ui->clampButton->isChecked()){
        if(arg1>ui->seekSlider->maximum()){
            ui->valueSpinBox->setValue(ui->seekSlider->maximum());
            return;
        }
        else if(arg1<ui->seekSlider->minimum()){
            ui->valueSpinBox->setValue(ui->seekSlider->minimum());
        }
    }
    _COORDINATE.setValue(m_model->updWorkingState(),SimTK::convertDegreesToRadians(arg1));
    updateModelGeometries();
}

void vsCoordinateDelegate::on_lockButton_toggled(bool checked)
{
    ui->valueSpinBox->setEnabled(!checked);
    ui->seekSlider->setEnabled(!checked);
    _COORDINATE.setLocked(m_model->updWorkingState(),checked);
    updateModelGeometries();
}

void vsCoordinateDelegate::on_clampButton_toggled(bool checked)
{
    _COORDINATE.setClamped(m_model->updWorkingState(),checked);
    updateModelGeometries();
}

void vsCoordinateDelegate::on_seekSlider_valueChanged(int value)
{
   ui->valueSpinBox->setValue(value);
}

void vsCoordinateDelegate::on_speedSpinBox_valueChanged(double arg1)
{
    _COORDINATE.setSpeedValue(m_model->updWorkingState(),arg1);
    updateModelGeometries();
}
