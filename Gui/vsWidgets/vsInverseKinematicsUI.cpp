#include "vsInverseKinematicsUI.h"
#include "ui_vsInverseKinematicsUI.h"

#include <vsTools/vsOpenSimTools.h>

#include <vsModeling/vsNavigatorModel.h>

#include <QDebug>
#include <QFileDialog>

vsInverseKinematicsUI::vsInverseKinematicsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsInverseKinematicsUI)
{
    ui->setupUi(this);


    connect(vsOpenSimTools::tools,&vsOpenSimTools::currentModelUpdated,this,&vsInverseKinematicsUI::onCurrentModelChanged);

    m_ikTool = new OpenSim::InverseKinematicsTool();

}

vsInverseKinematicsUI::~vsInverseKinematicsUI()
{
    delete ui;
}

void vsInverseKinematicsUI::onCurrentModelChanged()
{
    auto currentModel = vsOpenSimTools::tools->getNavigatorModel()->getActiveModel();

    m_currentModel =currentModel;
    m_ikTool->setModel(*m_currentModel);

    if(currentModel){
        qDebug() << "the current model exist";
        auto modelName = QString::fromStdString(currentModel->getName());
        auto sizeOfMarkersSet = currentModel->updMarkerSet().getSize();

        ui->currentModelTE->setPlainText(modelName);
        ui->markerSetTE->setPlainText(QString::number(sizeOfMarkersSet)+" markers");

    }else{
        ui->currentModelTE->setPlainText("No Model is available");
        ui->markerSetTE->setPlainText("0 markers");
    }

}



void vsInverseKinematicsUI::on_markerDataTB_clicked()
{
    if(!m_currentModel) return;
    QString markersDataFileName = QFileDialog::getOpenFileName(nullptr,"IK trial marker data","","marker data (*.trc)");
    if(markersDataFileName == "") return;

    m_ikTool->setMarkerDataFileName(markersDataFileName.toStdString());
}

void vsInverseKinematicsUI::on_timeRangeFromSB_valueChanged(double arg1)
{
    if(!m_ikTool) return;
    m_ikTool->setStartTime(arg1);
}

void vsInverseKinematicsUI::on_timeRangeToSB_valueChanged(double arg1)
{
    if(!m_ikTool) return;
    m_ikTool->setEndTime(arg1);
}

void vsInverseKinematicsUI::on_coordinateDataCB_toggled(bool checked)
{
    ui->coordinateDataTB->setEnabled(checked);
    ui->coordinateDataTE->setEnabled(checked);
}

void vsInverseKinematicsUI::on_coordinateDataTB_clicked()
{
    if(!m_currentModel) return;
    QString coordinateDataFileName = QFileDialog::getOpenFileName(nullptr,"Coordinate for IK trial","","coordinate data (*.mot,*.sto)");
    if(coordinateDataFileName == "") return;

    m_ikTool->setCoordinateFileName(coordinateDataFileName.toStdString());
}



void vsInverseKinematicsUI::on_merkerDataTE_textChanged()
{
    m_ikTool->setMarkerDataFileName(ui->markerSetTE->toPlainText().toStdString());
}

void vsInverseKinematicsUI::on_coordinateDataTE_textChanged()
{
    m_ikTool->setCoordinateFileName(ui->coordinateDataTE->toPlainText().toStdString());
}

void vsInverseKinematicsUI::on_motionFileTE_textChanged()
{
    m_ikTool->setOutputMotionFileName(ui->motionFileTE->toPlainText().toStdString());
}

void vsInverseKinematicsUI::on_motionFileTB_clicked()
{
    QString motionFileName = QFileDialog::getSaveFileName(nullptr,"motion file for output","","motion file (*.mot)");

    if(motionFileName == "") return;

    m_ikTool->setOutputMotionFileName(motionFileName.toStdString());

}
