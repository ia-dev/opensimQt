/***************************************************************************
 *   Forward Dynamics.Cpp File                                             *
 *   Copyright (C) 2020 by VisSim
 *   Authors: Venkata Guru Satvik Dondapati, Ibraheem Aldhamari            *
 *   Libraries used: m_FDtool:forwarddynamics.h
                   : AbstractTool.h                                        *

//ID variables
     m_ControlsFileName            ->Contains the time histories of the model controls (e.g., muscle excitations) to the muscles and/or joint torques
     m_InitialStateFileName        ->Contains the time histories of model states, including joint angles, joint speeds, muscle activations, muscle fiber lengths, and more.
     m_ResultDirectoryName         -> Additional data can be generated and written to files by adding analyses to the Forward Dynamics Tool. These analyses are specified in the setup file (subject01_Setup_Forward.xml) and are discussed in the Analyses section.
     m_PrecisionNumber             ->The precision of the decimal places used when writing results.
     m_ForceSetFileName            ->It contains settings, as described in How to Use the Forward Dynamics Tool, and refers to another settings file, gait2354_CMC_Actuators.xml, which contains a set of actuators that supplement the muscles of the model. Refer to Computed Muscle Control for more details.
     m_ExternalForceFileName       ->The file containing the external loads applied to the model during a simulation is specified using the property <external_loads_file>. Please refer to section Forward Dynamics Setup Files and XML Tags for details about the contents of this file.
     m_InitialTime and m_FinalTime ->The section for Time displays editable information that allows you to specify the start and end time for the forward simulation.
***************************************************************************/
#include "vsForwardDynamicsToolUI.h"
#include "ui_vsForwardDynamicsToolUI.h"

#include "qspinbox.h"
#include <QButtonGroup>
#include <stdio.h>
#include "iostream"

#include <QButtonGroup>
#include <QDebug>
#include <QDesktopServices>
#include <QFileDialog>
#include <QItemSelectionModel>

#include <vsTools/vsMotionsUtils.h>
#include <vsTools/vsOpenSimTools.h>

vsForwardDynamicsToolUI::vsForwardDynamicsToolUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsForwardDynamicsToolUI),
    m_CurrentModelInput(nullptr),
    m_FDtool(new OpenSim::ForwardTool),
    m_modelfilename(""),
    m_ControlsFileName(""),
    m_InitialStateFileName(""),
    m_PrecisionNumber(),
    m_ForceSetFileName(""),
    m_ExternalForceFileName(""),
    m_MaximumNumberOfSteps(),
    m_MaximumStepSize(),
    m_MinimumStepSize(),
    m_IntegratorErrorTolerance(),
    m_InitialTime(0),
    m_FinalTime(1)
{
    ui->setupUi(this);
    ui->AnalysesTable->setColumnCount(3);
    ui->AnalysesTable->setHorizontalHeaderLabels(QStringList() << tr("Enabled") << tr("Type") << tr("Name"));
    ui->AnalysesTable->setColumnWidth(0,100);
    ui->AnalysesTable->setColumnWidth(1,300);
    ui->AnalysesTable->setColumnWidth(2,300);
}

vsForwardDynamicsToolUI::~vsForwardDynamicsToolUI()
{
    delete ui;
}

OpenSim::Model *vsForwardDynamicsToolUI::CurrentModelInput() const
{
    return m_CurrentModelInput;
}

void vsForwardDynamicsToolUI::setCurrentModelInput(OpenSim::Model *CurrentModelInput)
{
    m_CurrentModelInput = CurrentModelInput;
    if(!m_CurrentModelInput) return;

    ui->RunBTN->setEnabled(true);

    m_InitialTime               = (m_FDtool->getInitialTime());
    m_FinalTime                 = m_FDtool->getFinalTime();
    m_MaximumNumberOfSteps      = m_FDtool->getMaximumNumberOfSteps();
    m_MaximumStepSize           = m_FDtool->getMaxDT();
    m_MinimumStepSize           = m_FDtool->getMinDT();
    m_IntegratorErrorTolerance  = m_FDtool->getErrorTolerance();
    m_OutputFileName            = m_FDtool->getResultsDir();
    ui->InitialTime->setValue((m_InitialTime));
    ui->FinalTimeRange->setValue((m_FinalTime));
    ui->DirectoryTE->setPlainText(QString::fromStdString(m_OutputFileName));
    ui->MaximumNumberOfStepsTE->setText(QString().setNum(m_MaximumNumberOfSteps));
    ui->MaximumumStepSizeTE->setText(QString::number(m_MaximumStepSize));
    ui->MinimumStepSizeTE->setText(QString().setNum(m_MinimumStepSize));
    ui->IntegratorErrorToleranceTE->setText(QString().setNum(m_IntegratorErrorTolerance));
    ui->ModelNameTE->setPlainText(QString::fromStdString(m_CurrentModelInput->getName()));
    ui->PrefixTE->setPlainText(QString::fromStdString(m_CurrentModelInput->getName()));
    ui->PrecisionTE->setPlainText("8");
}

void vsForwardDynamicsToolUI::on_ControlsBrowseTB_clicked()
{
    auto ControlsFileName = QFileDialog::getOpenFileName(nullptr,"Controls input data for forward dynamics tool(*.xml,*.sto)","","Controls input data for forward dynamics tool(*.xml *.sto)");
    if(ControlsFileName == "")return;

    m_ControlsFileName= ControlsFileName.toStdString();
    ui->ControlsTE->setPlainText(ControlsFileName);
}

void vsForwardDynamicsToolUI::on_InitialStateTB_clicked()
{
    auto InitialStateFileName = QFileDialog::getOpenFileName(nullptr,"Initial states data for forward dynamics tool(*.sto)","","Initial states data for forward dynamics tool(*.sto)");
    if(InitialStateFileName == "")return;

    m_InitialStateFileName= InitialStateFileName.toStdString();
    ui->InitialStateTE->setPlainText(InitialStateFileName);
}

void vsForwardDynamicsToolUI::on_DirectoryBrowsefile_clicked()
{
    auto OutputFileName = QFileDialog::getExistingDirectory(nullptr,"Motion data for Inverse Dynamics(*.mot *.sto)");
    if(OutputFileName == "") return;
    m_OutputFileName = OutputFileName.toStdString();
    ui->DirectoryTE->setPlainText(OutputFileName);

}

void vsForwardDynamicsToolUI::on_DirectoryTE_textChanged()
{
   m_OutputFileName = ui->DirectoryTE->toPlainText().toStdString();
}

void vsForwardDynamicsToolUI::on_ExternalLoadsSpecificationFileBrowseTB_clicked()
{
    auto ExternalForce = QFileDialog::getOpenFileName(nullptr,"External Forces(*.xml)","","External Forces(*.xml)");
    if(ExternalForce == "") return;
    m_ExternalForceFileName = ExternalForce.toStdString();
    ui->ExternalLoadsSpecificationFileTE->setPlainText(ExternalForce);
}

void vsForwardDynamicsToolUI::on_LoadBTN_clicked()
{
    //select the configuration file
    auto configFileName =  QFileDialog::getOpenFileName(nullptr,"ID tool setting file","","ID tool setting file (*.xml)");
    if(configFileName == "") return;

    ui->ExternalLoadsCB->setChecked(true);

    m_FDtool = new OpenSim::ForwardTool(configFileName.toStdString(),true, false);
    m_FDtool->setModel(*m_CurrentModelInput);


    //convert the relative path to an absolute one (for the registered files)
    auto configFileDir = QFileInfo(configFileName).dir();
    m_modelfilename             = QString::fromStdString(m_FDtool->getModelFilename()).toStdString();
    m_ControlsFileName          = configFileDir.cleanPath(configFileDir.absoluteFilePath(QString::fromStdString(m_FDtool->getControlsFileName()))).toStdString();
    m_InitialStateFileName      = configFileDir.cleanPath(configFileDir.absoluteFilePath(QString::fromStdString(m_FDtool->getStatesFileName()))).toStdString();
    m_InitialTime               = m_FDtool->getInitialTime();
    m_FinalTime                 = m_FDtool->getFinalTime();
    m_OutputFileName            = configFileDir.cleanPath(configFileDir.absoluteFilePath(QString::fromStdString(m_FDtool->getResultsDir()))).toStdString();
    m_PrecisionNumber           = m_FDtool->getOutputPrecision();
    OpenSim::Array<std::string> ForceFileName = m_FDtool->getForceSetFiles();
    m_ForceSetFileName          = configFileDir.cleanPath(configFileDir.absoluteFilePath(QString::fromStdString(ForceFileName[0]))).toStdString();
    m_ExternalForceFileName     = configFileDir.cleanPath(configFileDir.absoluteFilePath(QString::fromStdString(m_FDtool->getExternalLoadsFileName()))).toStdString();
    m_MaximumNumberOfSteps      = m_FDtool->getMaximumNumberOfSteps();
    m_MaximumStepSize           = m_FDtool->getMaxDT();
    m_MinimumStepSize           = m_FDtool->getMinDT();
    m_IntegratorErrorTolerance  = m_FDtool->getErrorTolerance();

    for(int i = ui->AnalysesTable->rowCount() -1; i >=0; --i)
    {
        ui->AnalysesTable->removeRow(i);
    }
    OpenSim::Array<std::string> arryVals;
    m_FDtool->getAnalysisSet().getNames(arryVals);
    OpenSim::Array<bool> arryOns;
    arryOns = m_FDtool->getAnalysisSet().getOn();
    for(int rowcount=0; rowcount < arryVals.size(); rowcount++)
    {
        ui->AnalysesTable->insertRow(rowcount);
        QCheckBox* box = new QCheckBox(this);
        box->setChecked(arryOns[rowcount]);
        ui->AnalysesTable->setCellWidget(rowcount,0,box);

        QTableWidgetItem* Names  = new QTableWidgetItem(arryVals[rowcount].c_str());
        ui->AnalysesTable->setItem(rowcount,1,Names);

        QTableWidgetItem* Types  = new QTableWidgetItem(arryVals[rowcount].c_str());
        ui->AnalysesTable->setItem(rowcount,2,Types);
    }

    std::cout <<"modelfilename" << m_FDtool->getModelFilename() << std::endl;
    std::cout <<"ForceFileName:" << m_FDtool->getForceSetFiles() << std::endl;
    std::cout <<"ControlFileName" << m_ControlsFileName << std::endl;
    std::cout <<"Intial Time" << m_InitialTime << std::endl;
    std::cout <<"Final Time" << m_FinalTime << std::endl;
    std::cout <<"Precision Number:" << m_PrecisionNumber << std::endl;
    std::cout <<"result directory File Name:" << m_OutputFileName << std::endl;
    std::cout <<"max no of steps:" << m_FDtool->getMaximumNumberOfSteps() << std::endl;
    std::cout <<"max no of steps:" << m_MaximumNumberOfSteps << std::endl;
    std::cout <<"min Step Size:" << m_MaximumStepSize << std::endl;
    std::cout <<"min Step Size:" << m_MinimumStepSize << std::endl;
    std::cout <<"Integrator Error Tolerance:" << m_IntegratorErrorTolerance << std::endl;
    std::cout <<"ForceFileName:" << m_ExternalForceFileName << std::endl;

   ui->ControlsTE->setPlainText(QString::fromStdString(m_ControlsFileName));
   ui->InitialStateTE->setPlainText(QString::fromStdString(m_InitialStateFileName));
   ui->InitialTime->setValue((m_InitialTime));
   ui->FinalTimeRange->setValue((m_FinalTime));
   ui->DirectoryTE->setPlainText(QString::fromStdString(m_OutputFileName));
   ui->PrecisionTE->setPlainText(QString().setNum(m_PrecisionNumber));
   ui->AdditionalForceSetFilesTE->setPlainText(QString::fromStdString(m_ForceSetFileName));
   ui->ExternalLoadsSpecificationFileTE->setPlainText(QString::fromStdString(m_ExternalForceFileName));
   ui->MaximumNumberOfStepsTE->setText(QString().setNum(m_MaximumNumberOfSteps));
   ui->MaximumumStepSizeTE->setText(QString::number(m_MaximumStepSize));
   ui->MinimumStepSizeTE->setText(QString().setNum(m_MinimumStepSize));
   ui->IntegratorErrorToleranceTE->setText(QString().setNum(m_IntegratorErrorTolerance));
}

void vsForwardDynamicsToolUI::on_RunBTN_clicked()
{
    // Check the output value is set  
    if(m_OutputFileName=="")
    {
        vsOpenSimTools::tools->log("No output file was specified for the motion","vsForwardDynamicsToolUI",vsOpenSimTools::Error);
        return;
    }
    vsOpenSimTools::tools->log("Running the FDTool ...");
    std::cout << "-------------------------------------------------------"<< std::endl;
    std::cout << "    Forward Dynamic Tools has been started ......... "<< std::endl;
    std::cout << "-------------------------------------------------------"<< std::endl;
    std::cout <<"Model Name                 :" << m_CurrentModelInput->getName() << std::endl;
    std::cout <<"Controls filename          :" << m_ControlsFileName << std::endl;
    std::cout <<"Initial State File Name    :" << m_InitialStateFileName << std::endl;
    std::cout <<"Time Range                 :" << m_InitialTime <<" to " << m_FinalTime << "seconds!"<<std::endl;
    std::cout <<"Precision Number           :" << m_PrecisionNumber << std::endl;
    std::cout <<"Force Set File Name        :" << m_ForceSetFileName << std::endl;
    std::cout <<"External Force File Name   :" << m_ExternalForceFileName << std::endl;
    std::cout <<"Maximum Number Of Steps    :" << m_MaximumNumberOfSteps << std::endl;
    std::cout <<"Maximum Step Size          :" << m_MaximumStepSize << std::endl;
    std::cout <<"Minimum Step Size          :" << m_MinimumStepSize << std::endl;
    std::cout <<"Integrator Error Tolerance :" << m_IntegratorErrorTolerance << std::endl;
    std::cout <<"Result Directory           :" << m_FDtool->getResultsDir() << std::endl;
    std::cout <<"Result Directory           :" << m_OutputFileName << std::endl;

    m_FDtool->setControlsFileName(m_ControlsFileName);
    m_FDtool->setStatesFileName(m_InitialStateFileName);
    m_FDtool->setInitialTime(m_InitialTime);
    m_FDtool->setFinalTime(m_FinalTime);
    m_FDtool->setOutputPrecision(m_PrecisionNumber);
    m_FDtool->setResultsDir(m_OutputFileName);
    m_FDtool->setExternalLoadsFileName(m_ExternalForceFileName);
    m_FDtool->setMaximumNumberOfSteps(m_MaximumNumberOfSteps);
    m_FDtool->setMaxDT(m_MaximumStepSize);
    m_FDtool->setMinDT(m_MinimumStepSize);
    m_FDtool->setErrorTolerance(m_IntegratorErrorTolerance);

    try {
         m_FDtool->run();
    } catch (const std::exception & ex ) {
        std::cout << "Error when running forward Dynamic Tool ......... "<< std::endl;
        std::cout << ex.what() << std::endl;
    }
    std::string outputfile = m_FDtool->getResultsDir();
    outputfile.append("/");
    outputfile.append(m_FDtool->getName());
    outputfile.append("_states.sto");

    try {
        vsOpenSimTools::tools->log("Running the FDTool ...");
        vsOpenSimTools::tools->log("Execution for the FD tool has finnished ...");
        OpenSim::Storage *storage = new OpenSim::Storage( outputfile);
        storage->setName("Results");
        vsMotionsUtils::getInstance()->activeModel = m_CurrentModelInput;
        vsMotionsUtils::getInstance()->loadMotionStorage(storage,true, outputfile);

        }   catch (const std::exception & ex ) {
            std::cout << "Error with path "<< std::endl;
            std::cout << ex.what() << std::endl;
        }
}

void vsForwardDynamicsToolUI::on_SaveBTN_clicked()
{
    //Open A File Dialog
        auto savingFileName = QFileDialog::getSaveFileName(nullptr,"Setting file for forward dynamics tool (*.xml)","","Setting file for forward dynamics tool (*.xml)");
        if(savingFileName == "") return;

    //get the relative paths to the saveFileName and/get the correct path
        QFileInfo savingFileInfo(savingFileName);
        QDir savingDir  = savingFileInfo.dir();
        auto relativeControlFile                = savingDir.relativeFilePath(QString::fromStdString(m_ControlsFileName));
        auto relativeInitialStateFile           = savingDir.relativeFilePath(QString::fromStdString(m_InitialStateFileName));
        auto relativeResultDirectoryFile        = savingDir.relativeFilePath(QString::fromStdString(m_OutputFileName));
        auto relativePrecision                  = savingDir.relativeFilePath(QString::number(m_PrecisionNumber));
        auto relativeAdditionalForceSetFiles    = savingDir.relativeFilePath(QString::fromStdString(m_ForceSetFileName));
        auto relativeMaximumNumberOfSteps       = savingDir.relativeFilePath(QString::number(m_MaximumNumberOfSteps));
        auto relativeMaximumStepSize            = savingDir.relativeFilePath(QString::number(m_MaximumStepSize));
        auto relativeMinimumStepSize            = savingDir.relativeFilePath(QString::number(m_MinimumStepSize));
        auto relativeIntegratorErrorTolerance   = savingDir.relativeFilePath(QString::number(m_IntegratorErrorTolerance));
        auto relativeModelFile                  = savingDir.relativeFilePath(QString::fromStdString(m_CurrentModelInput->getInputFileName()));

    //load the current tasks before saving
        m_FDtool->setInitialTime(m_InitialTime);
        m_FDtool->setFinalTime(m_FinalTime);
        m_FDtool->setControlsFileName(relativeControlFile .toStdString());
        m_FDtool->setStatesFileName(relativeInitialStateFile.toStdString());
        m_FDtool->setMaximumNumberOfSteps(m_MaximumNumberOfSteps);
        m_FDtool->setMaxDT(m_MaximumStepSize);
        m_FDtool->setMinDT(m_MinimumStepSize);
        m_FDtool->setErrorTolerance(m_IntegratorErrorTolerance);
        m_FDtool->print(savingFileName.toStdString());
}

void vsForwardDynamicsToolUI::on_CloseBTN_clicked()
{
    this->close();
}

void vsForwardDynamicsToolUI::on_HelpBTN_clicked()
{
    QDesktopServices::openUrl(QUrl("https://simtk-confluence.stanford.edu/display/OpenSim/Forward+Dynamics"));
}



