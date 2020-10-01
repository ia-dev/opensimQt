#include "vsInverseKinematicsUI.h"
#include "ui_vsInverseKinematicsUI.h"

#include <QDebug>
#include <QFileDialog>

vsInverseKinematicsUI::vsInverseKinematicsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsInverseKinematicsUI),
    m_currentModel(nullptr),
    m_ikTool(new OpenSim::InverseKinematicsTool()),
    m_markersIKTasksModel(new vsMarkerTasksModel()),
    m_markersFileName(""),
    m_ikStartTime(0),
    m_ikEndTime(0),
    m_coordinatesFileName(""),
    m_outputFile("")
{
    ui->setupUi(this);

    ui->ikMarkersTable->setModel(m_markersIKTasksModel);
    ui->ikMarkersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

vsInverseKinematicsUI::~vsInverseKinematicsUI()
{
    delete ui;
}

OpenSim::Model *vsInverseKinematicsUI::currentModel() const
{
    return m_currentModel;
}

void vsInverseKinematicsUI::setCurrentModel(OpenSim::Model *currentModel)
{
    m_currentModel = currentModel;
    if(!m_currentModel) return;

    ui->runBTN->setEnabled(true);

    //You need to convert the string to QString instead of std::string
    ui->modelNameTE->setPlainText(QString::fromStdString(m_currentModel->getName()));
    auto markersSetSize = m_currentModel->updMarkerSet().getSize();

    ui->markersSetTE->setPlainText(QString::number(markersSetSize)+" markers");

    m_markersIKTasksModel->updateTasks(m_currentModel);

}

void vsInverseKinematicsUI::on_markerFileTB_clicked(){

    auto markersFileName =  QFileDialog::getOpenFileName(nullptr,"IK trial marker data","","IK trial marker data (*.trc)");
    if(markersFileName == "") return;

    m_markersFileName = markersFileName.toStdString();

    ui->markerDateFileTE->setPlainText(markersFileName);

}

void vsInverseKinematicsUI::on_markerDateFileTE_textChanged()
{
    auto markersDataText  =  ui->markerDateFileTE->toPlainText();
//    if(markersDataText == "") return;
    //actually we need the text to be empty when there is no text on the text edit

    m_markersFileName =  markersDataText.toStdString();
}

void vsInverseKinematicsUI::on_trialRangefrom_valueChanged(double arg1)
{
    m_ikStartTime = arg1;
}

void vsInverseKinematicsUI::on_trialRangeTo_valueChanged(double arg1)
{
    m_ikEndTime = arg1;
}

void vsInverseKinematicsUI::on_coordinateDataCB_toggled(bool checked)
{
    ui->coordianteFileTB->setEnabled(checked);
    ui->coordinateFileTE->setEnabled(checked);
}

void vsInverseKinematicsUI::on_coordinateFileTE_textChanged()
{
    m_coordinatesFileName = ui->coordinateFileTE->toPlainText().toStdString();
}

void vsInverseKinematicsUI::on_coordianteFileTB_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(nullptr,"Coordintes of IK trial","","Coordinates (*.mot *.sto)");
    if(fileName == "") return;

    m_coordinatesFileName = fileName.toStdString();
    ui->coordinateFileTE->setPlainText(fileName);
}

void vsInverseKinematicsUI::on_motionFileTB_clicked()
{
    auto outputFileName = QFileDialog::getSaveFileName(nullptr,"Result motion file for IK","","Result Motion file (*.mot)");

    if(outputFileName == "") return;

    m_outputFile = outputFileName.toStdString();

    ui->motionFileName->setPlainText(outputFileName);

}

void vsInverseKinematicsUI::on_motionFileName_textChanged()
{
    m_outputFile = ui->motionFileName->toPlainText().toStdString();
}

void vsInverseKinematicsUI::on_runBTN_clicked()
{

    m_ikTool->setModel(*m_currentModel);
    m_ikTool->setMarkerDataFileName(m_markersFileName);
    m_ikTool->setStartTime(m_ikStartTime);
    m_ikTool->setEndTime(m_ikEndTime);
    if(ui->coordinateDataCB->isChecked()) m_ikTool->setCoordinateFileName(m_coordinatesFileName);
    m_ikTool->setOutputMotionFileName(m_outputFile);

    std::cout << "IK Tool info :: " << std::endl
              <<" Model Name : " << m_currentModel->getName() << std::endl
              << "markers File " << m_markersFileName << std::endl
              << "range : " << m_ikStartTime << " > "  << m_ikEndTime << std::endl
              << "coordinate file: " << m_coordinatesFileName << std::endl
              << "output file : " << m_outputFile;
}
