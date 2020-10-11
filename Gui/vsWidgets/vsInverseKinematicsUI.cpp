#include "vsInverseKinematicsUI.h"
#include "ui_vsInverseKinematicsUI.h"

#include <QDebug>
#include <QFileDialog>
#include <QItemSelectionModel>

vsInverseKinematicsUI::vsInverseKinematicsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsInverseKinematicsUI),
    m_currentModel(nullptr),
    m_ikTool(new OpenSim::InverseKinematicsTool()),
    m_markersIKTasksModel(new vsMarkerTasksModel()),
    m_coordinateIKTasksModel(new vsIKCoordinateModel()),
    m_markersFileName(""),
    m_ikStartTime(0),
    m_ikEndTime(0),
    m_coordinatesFileName(""),
    m_outputFile("")
{
    ui->setupUi(this);

    ui->ikMarkersTable->setModel(m_markersIKTasksModel);
    ui->ikCoordiantesTable->setModel(m_coordinateIKTasksModel);
    ui->ikMarkersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->ikMarkersTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    ui->ikCoordiantesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->ikCoordiantesTable->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);

    connect(ui->ikMarkersTable->selectionModel(),&QItemSelectionModel::selectionChanged,m_markersIKTasksModel,[=](){
        m_markersIKTasksModel->selectionModelChanged(ui->ikMarkersTable->selectionModel()->selectedRows());
        ui->enableAllCB->setChecked(m_markersIKTasksModel->getAllEnabled());
        ui->disableAllCB->setChecked(m_markersIKTasksModel->getAllDisabled());
    });

    connect(ui->ikCoordiantesTable->selectionModel(),&QItemSelectionModel::selectionChanged,m_coordinateIKTasksModel,[=](){
        m_coordinateIKTasksModel->selectionModelChanged(ui->ikCoordiantesTable->selectionModel()->selectedRows());
        ui->enableAllCB->setChecked(m_coordinateIKTasksModel->getAllEnabled());
        ui->disableAllCB->setChecked(m_coordinateIKTasksModel->getAllDisabled());
    });

    connect(m_markersIKTasksModel,&vsMarkerTasksModel::uiUpdated,[=](){

        ui->enableAllCB->setChecked(m_markersIKTasksModel->getAllEnabled());
        ui->enableAllCB->setEnabled(!m_markersIKTasksModel->getAllEnabled());

        ui->disableAllCB->setChecked(m_markersIKTasksModel->getAllDisabled());
        ui->disableAllCB->setEnabled(!m_markersIKTasksModel->getAllDisabled());

    });

    connect(m_coordinateIKTasksModel,&vsIKCoordinateModel::uiUpdated,[=](){

        //TODO Set it to be the last to be changed

        ui->enableAllCB->setChecked(m_coordinateIKTasksModel->getAllEnabled());
        ui->enableAllCB->setEnabled(!m_coordinateIKTasksModel->getAllEnabled());

        ui->disableAllCB->setChecked(m_coordinateIKTasksModel->getAllDisabled());
        ui->disableAllCB->setEnabled(!m_coordinateIKTasksModel->getAllDisabled());

    });

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

    m_ikTool->setModel(*m_currentModel);

    m_markersIKTasksModel->updateTasks(m_currentModel);
    m_coordinateIKTasksModel->updateTasks(m_currentModel);
    

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

    QFileInfo fileInfo(QString::fromStdString(m_markersFileName));

    if(m_markersFileName != "" && fileInfo.exists()){
        ui->fromFileTE->setPlainText(QString::fromStdString(m_markersFileName));
        // that should load the IK tasks to the IK tool
        m_ikTool->setMarkerDataFileName(m_markersFileName);

        m_markersIKTasksModel->loadFromIKTool(m_ikTool);
        m_coordinateIKTasksModel->loadFromIKTool(m_ikTool);
    }
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

void vsInverseKinematicsUI::on_enableAllCB_toggled(bool checked)
{
    //TODO depending on the last one selected
    if(checked){
        m_markersIKTasksModel->enableAllSelected();
        m_coordinateIKTasksModel->enableAllSelected();
    }
}

void vsInverseKinematicsUI::on_disableAllCB_toggled(bool checked)
{
    if(checked){
        m_markersIKTasksModel->disableAllSelected();
        m_coordinateIKTasksModel->disableAllSelected();
    }
}

void vsInverseKinematicsUI::on_weightFSP_valueChanged(double arg1)
{
    m_markersIKTasksModel->updateSelectedRowsWeight(arg1);
    m_coordinateIKTasksModel->updateSelectedRowsWeight(arg1);
}
