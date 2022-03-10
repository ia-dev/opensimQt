/***************************************************************************
 *   AnalyzeTool.Cpp File                                                  *
 *   Copyright (C) 2020 by VisSim
 *   Authors: Venkata Guru Satvik Dondapati, Ibraheem Aldhamari            *
 *   Libraries used: m_FDtool:forwarddynamics.h
                   : AbstractTool.h                                        *

//ID variables
     m_ControlsFileName            ->Contains the time histories of the model controls (e.g., muscle excitations) to the muscles and/or joint torques
     m_InitialStateFileName        ->Contains the time histories of model states, including joint angles, joint speeds, muscle activations, muscle fiber lengths, and more.
     m_ResultDirectoryName         -> Additional data can be generated and written to files by adding analyses to the Forward Dynamics Tool. These analyses are specified in the setup file (subject01_Setup_Forward.xml) and are discussed in the Analyses section.
     m_PrecisionNumber             -> The precision of the decimal places used when writing results.
     m_ForceSetFileName            ->It contains settings, as described in How to Use the Forward Dynamics Tool, and refers to another settings file, gait2354_CMC_Actuators.xml, which contains a set of actuators that supplement the muscles of the model. Refer to Computed Muscle Control for more details.
     m_ExternalForceFileName       ->The file containing the external loads applied to the model during a simulation is specified using the property <external_loads_file>. Please refer to section Forward Dynamics Setup Files and XML Tags for details about the contents of this file.
     m_InitialTime and m_FinalTime ->The section for Time displays editable information that allows you to specify the start and end time for the forward simulation.
***************************************************************************/
#include "vsAnalyzeToolUI.h"
#include "ui_vsAnalyzeToolUI.h"
#include "vsExterenalForcesUI.h"

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

vsAnalyzeToolUI::vsAnalyzeToolUI(QWidget *parent) :
    QWidget(parent),
    m_FDtool(new OpenSim::ForwardTool),
    ui(new Ui::vsAnalyzeToolUI),
    m_CurrentNameInput(nullptr),
    m_ControlsFileName(""),
    m_StateFileName(""),
    m_FromFile(""),
    m_ActiveAnalysesNames(""),
    m_ForceSetFileName(""),
    m_ExternalForceFileName(""),
    m_InitialTime(0),
    m_FinalTime(1)

{
    ui->setupUi(this);
    ui->AnalysesTable->setColumnCount(3);
    ui->AnalysesTable->setHorizontalHeaderLabels(QStringList() << tr("Enabled") << tr("Type") << tr("Name"));
    ui->AnalysesTable->setColumnWidth(0,100);
    ui->AnalysesTable->setColumnWidth(1,250);
    ui->AnalysesTable->setColumnWidth(2,250);  
}

vsAnalyzeToolUI::~vsAnalyzeToolUI()
{
    delete ui;
}

OpenSim::Model *vsAnalyzeToolUI::CurrentInput() const
{
    return m_CurrentNameInput;
}

void vsAnalyzeToolUI::setCurrentInput(OpenSim::Model *CurrentInput)
{
    m_CurrentNameInput = CurrentInput;
    if(!m_CurrentNameInput) return;

    ui->RunPB->setEnabled(true);
    ui->AppendtomodelsforcesetRB->setChecked(true);
    ui->FromFileRB->setEnabled(false);
    ui->LoadedMotionRB->setEnabled(false);
    ui->FilterCoordinatesCB->setEnabled(false);
    ui->FromFileNameTE->setEnabled(false);
    ui->FilterCoordinatesNumberTE->setEnabled(false);
    ui->LoadedMotionCB->setEnabled(false);
    ui->FromFileRB->setChecked(true);
    ui->ControlsFileNameTE->setEnabled(false);
    ui->ControlsBrowseTB->setEnabled(false);
    ui->StateFileNameTE->setEnabled(false);
    ui->StatesBrowseTB->setEnabled(false);
    ui->ModelNameTE->setPlainText(QString::fromStdString(m_CurrentNameInput->getName()));
    ui->PrefixTE->setPlainText(QString::fromStdString(m_CurrentNameInput->getName()));
    m_OutputFileName            = m_FDtool->getResultsDir();
    ui->DirectoryTE->setPlainText(QString::fromStdString(m_OutputFileName));
    ui->PrecisionTE->setPlainText("8");
    ui->InitialTimeSB->setValue((m_InitialTime));
    ui->FinalTimeSB->setValue((m_FinalTime));
    ui->LoadedMotionCB->clear();

    ui->LoadedMotionCB->clear();
    if(m_CurrentNameInput !=NULL)
    {
     vsNavigatorNode *modelNode = vsOpenSimTools::tools->getObjectNode(m_CurrentNameInput);
     if(modelNode == NULL)
         return;
     vsNavigatorNode *motionsNode = modelNode->findChildNode("Motions");
     if(motionsNode == NULL)
         return;
     QList<vsNavigatorNode*> childNodes = motionsNode->childNodes;

     if ( childNodes.size() > 0)
     {
          ui->MotionRB->setChecked(true);
          ui->LoadedMotionRB->setChecked(true);
          ui->LoadedMotionRB->setEnabled(true);
     }
     else
     {

          ui->LoadedMotionRB->setChecked(false);
          ui->LoadedMotionRB->setEnabled(false);
          ui->StatesRB->setChecked(false);

          ui->StateFileNameTE->setEnabled(false);
          ui->StatesBrowseTB->setEnabled(false);

          ui->FromFileRB->setEnabled(false);
          ui->FromFileBrowseTB->setEnabled(false);

          ui->FilterCoordinatesCB->setEnabled(false);
          ui->FromFileNameTE->setEnabled(false);
          ui->FilterCoordinatesNumberTE->setEnabled(false);
          ui->LoadedMotionCB->setEnabled(false);
     }

     for(int i=0; i < childNodes.size(); i++)
        {
          QString LoadMotionName = childNodes[i]->FilePath;
          std::cout << "LoadMotionName"<< LoadMotionName.toStdString()<< std::endl;
          QVariant varient = QVariant::fromValue(LoadMotionName);
          ui->LoadedMotionCB->addItem(QString(childNodes[i]->displayName), varient);
        }
     }

}

void vsAnalyzeToolUI::on_ControlsCB_toggled(bool checked)
{
    ui->ControlsFileNameTE->setEnabled(false);
    ui->ControlsBrowseTB->setEnabled(false);
    if(checked)
    {
        ui->ControlsFileNameTE->setEnabled(true);
        ui->ControlsBrowseTB->setEnabled(true);
    }
}

void vsAnalyzeToolUI::on_ControlsBrowseTB_clicked()
{
    auto ControlsFileName = QFileDialog::getOpenFileName(nullptr,"Controls input data for analyze tool(*.xml,*.sto)","","Controls input data for analyze tool(*.xml *.sto)");
    if(ControlsFileName == "")return;

    m_ControlsFileName= ControlsFileName.toStdString();
    ui->ControlsFileNameTE->setPlainText(ControlsFileName);
}

void vsAnalyzeToolUI::on_StatesRB_toggled(bool checked)
{
    ui->StateFileNameTE->setEnabled(true);
    ui->StatesBrowseTB->setEnabled(true);
    ui->FromFileRB->setEnabled(false);
    ui->FromFileBrowseTB->setEnabled(false);
    ui->LoadedMotionRB->setEnabled(false);
    ui->FilterCoordinatesCB->setEnabled(false);
    ui->FromFileNameTE->setEnabled(false);
    ui->FilterCoordinatesNumberTE->setEnabled(false);
    ui->LoadedMotionCB->setEnabled(false);
}


void vsAnalyzeToolUI::on_StatesBrowseTB_clicked()
{
    auto InitialStateFileName = QFileDialog::getOpenFileName(nullptr,"States data for analyze tool(*.sto)","","States data for analyze tool(*.sto)");
    if(InitialStateFileName == "")return;

    m_StateFileName= InitialStateFileName.toStdString();
    ui->StateFileNameTE->setPlainText(InitialStateFileName);
}

void vsAnalyzeToolUI::on_MotionRB_toggled(bool checked)
{
    ui->FromFileRB->setEnabled(true);
    ui->FromFileNameTE->setEnabled(true);
    ui->FromFileBrowseTB->setEnabled(true);
    ui->FilterCoordinatesCB->setEnabled(true);
    ui->FilterCoordinatesNumberTE->setEnabled(true);
    ui->LoadedMotionCB->setEnabled(true);

    if( ui->LoadedMotionCB->count() ==0)
    {
      ui->LoadedMotionRB->setEnabled(false);
      ui->FromFileRB->setChecked(true);
    }
    else
    {
      ui->LoadedMotionRB->setEnabled(true);
      ui->LoadedMotionRB->setChecked(true);
    }
      ui->StateFileNameTE->setEnabled(false);
      ui->StatesBrowseTB->setEnabled(false);


}


void vsAnalyzeToolUI::on_FromFileBrowseTB_clicked()
{
    auto fromfile = QFileDialog::getOpenFileName(nullptr,"External forces","","External forces(*.mot *.sto)");
    if(fromfile == "")return;

    m_FromFile= fromfile.toStdString();
    ui->FromFileNameTE->setPlainText(fromfile);
}

void vsAnalyzeToolUI::on_FilterCoordinatesCB_toggled(bool checked)
{
    ui->FilterCoordinatesNumberTE->setEnabled(false);
    ui->FilterCoordinatesNumberTE->setPlainText("");
    if(checked)
    {
        ui->FilterCoordinatesNumberTE->setEnabled(true);
        ui->FilterCoordinatesNumberTE->setPlainText("6");
    }
}

void vsAnalyzeToolUI::on_ActiveAnalysesEditTB_clicked()
{

}


void vsAnalyzeToolUI::on_DirectoryTB_clicked()
{
    auto OutputFileName = QFileDialog::getExistingDirectory(nullptr,"Setting file for Analyze tool(*.xlm)");
    if(OutputFileName == "") return;
    m_OutputFileName = OutputFileName.toStdString();
    ui->DirectoryTE->setPlainText(OutputFileName);

}


void vsAnalyzeToolUI::on_ExternalLoadsSpecificationFileTB_clicked()
{
    auto ExternalForce = QFileDialog::getOpenFileName(nullptr,"External Forces(*.xml)","","External Forces(*.xml)");
    if(ExternalForce == "") return;
    m_ExternalForceFileName = ExternalForce.toStdString();
    ui->ExternalLoadsSpecificationFileTE->setPlainText(ExternalForce);
}


void vsAnalyzeToolUI::on_ExternalLoadsSpecificationFileEditTB_clicked()
{
    if(!m_ExternalForceFileName.empty())
    {
        std::cout <<"externalfromfile 1:" << m_ExternalForceFileName << std::endl;
        vsExterenalForcesUI* externalforce = new vsExterenalForcesUI(this, m_ExternalForceFileName.c_str());
        std::string txt = ui->ExternalLoadsSpecificationFileTE->toPlainText().toStdString();
        std::cout <<"externalfromfile 2:" << m_ExternalForceFileName << std::endl;
        externalforce->setModal(true);
        externalforce->exec();
    } else{
            vsExterenalForcesUI* externalforce = new vsExterenalForcesUI(this);
            std::cout <<"externalfromfile:" << m_ExternalForceFileName << std::endl;
            externalforce->setModal(true);
            externalforce->exec();
        }
}


void vsAnalyzeToolUI::on_LoadPB_clicked()
{
    //select the configuration file
    auto configFileName =  QFileDialog::getOpenFileName(nullptr,"ID tool setting file","","ID tool setting file (*.xml)");
    if(configFileName == "") return;

    ui->ControlsCB->setChecked(true);
    ui->ExternalLoadsCB->setChecked(true);
    ui->StatesRB->setChecked(true);

    m_FDtool = new OpenSim::ForwardTool(configFileName.toStdString(),true, false);
    m_FDtool->setModel(*m_CurrentNameInput);


    //convert the relative path to an absolute one (for the registered files)
    auto configFileDir = QFileInfo(configFileName).dir();
    m_ControlsFileName          = configFileDir.cleanPath(configFileDir.absoluteFilePath(QString::fromStdString(m_FDtool->getControlsFileName()))).toStdString();
    m_StateFileName             = configFileDir.cleanPath(configFileDir.absoluteFilePath(QString::fromStdString(m_FDtool->getStatesFileName()))).toStdString();
    m_InitialTime               = (m_FDtool->getInitialTime());
    m_FinalTime                 = m_FDtool->getFinalTime();
    m_OutputFileName            = configFileDir.cleanPath(configFileDir.absoluteFilePath(QString::fromStdString(m_FDtool->getResultsDir()))).toStdString();
    m_PrecisionNumber           = m_FDtool->getOutputPrecision();
    OpenSim::Array<std::string> ForceFileName = m_FDtool->getForceSetFiles();
    m_ForceSetFileName          = configFileDir.cleanPath(configFileDir.absoluteFilePath(QString::fromStdString(ForceFileName[0]))).toStdString();
    m_ExternalForceFileName     = configFileDir.cleanPath(configFileDir.absoluteFilePath(QString::fromStdString(m_FDtool->getExternalLoadsFileName()))).toStdString();

    for(int i = ui->AnalysesTable->rowCount() -1; i >=0; --i)
    {
        ui->AnalysesTable->removeRow(i);
    }
    OpenSim::Array<std::string> AnalysisNames;
    m_FDtool->getAnalysisSet().getNames(AnalysisNames);
    OpenSim::Array<bool> arryOns;
    arryOns = m_FDtool->getAnalysisSet().getOn();
    for(int rowcount=0; rowcount < AnalysisNames.size(); rowcount++)
    {
        ui->AnalysesTable->insertRow(rowcount);
        QCheckBox* box = new QCheckBox(this);
        box->setChecked(arryOns[rowcount]);
        ui->AnalysesTable->setCellWidget(rowcount,0,box);

        QTableWidgetItem* Names  = new QTableWidgetItem(AnalysisNames[rowcount].c_str());
        ui->AnalysesTable->setItem(rowcount,1,Names);

        QTableWidgetItem* Types  = new QTableWidgetItem(AnalysisNames[rowcount].c_str());
        ui->AnalysesTable->setItem(rowcount,2,Types);
    }

    QString AnalizeNames;

    for(int i=0; i<AnalysisNames.size(); i++)
    {

       QString Names  =  AnalysisNames[i].c_str();
       AnalizeNames.append(Names);
       AnalizeNames.append(" ");

    }
    ui->ActiveAnalysesTE->setPlainText(AnalizeNames);

    std::cout <<"AnalysisSetNames:" << AnalysisNames << std::endl;
    std::cout <<"ForceFileName:" << m_FDtool->getForceSetFiles() << std::endl;
    std::cout <<"ControlFileName" << m_ControlsFileName << std::endl;
    std::cout <<"Intial Time" << m_InitialTime << std::endl;
    std::cout <<"Final Time" << m_FinalTime << std::endl;
    std::cout <<"Precision Number:" << m_PrecisionNumber << std::endl;
    std::cout <<"result directory File Name:" << m_OutputFileName << std::endl;
    std::cout <<"ForceFileName:" << m_ExternalForceFileName << std::endl;

    ui->ControlsFileNameTE->setPlainText(QString::fromStdString(m_ControlsFileName));
    ui->StateFileNameTE->setPlainText(QString::fromStdString(m_StateFileName));
    ui->InitialTimeSB->setValue((m_InitialTime));
    ui->FinalTimeSB->setValue((m_FinalTime));
    ui->DirectoryTE->setPlainText(QString::fromStdString(m_OutputFileName));
    ui->PrecisionTE->setPlainText(QString().setNum(m_PrecisionNumber));
    ui->AdditionalforcesetfilsTE->setPlainText(QString::fromStdString(m_ForceSetFileName));
    ui->ExternalLoadsSpecificationFileTE->setPlainText(QString::fromStdString(m_ExternalForceFileName));

    //ui->ActiveAnalysesTE->setPlainText(AnalysisNames);

}


void vsAnalyzeToolUI::on_RunPB_clicked()
{
    if(ui->LoadedMotionRB->isChecked() == true)
    {
        int index = ui->LoadedMotionCB->currentIndex();
        QVariant variant = ui->LoadedMotionCB->itemData(index);
        QString strfileName = variant.toString();
        std::cout << "LodFilename"<< strfileName.toLatin1().data()<< std::endl;
    }

    // Check the output value is set
    if(m_OutputFileName=="")
    {
        vsOpenSimTools::tools->log("No output file was specified for the motion","vsAnalyzeToolUI",vsOpenSimTools::Error);
        return;
    }
    vsOpenSimTools::tools->log("Running the FDTool ...");
    std::cout << "-------------------------------------------------------"<< std::endl;
    std::cout << "    Analyze Tools has been started ......... "<< std::endl;
    std::cout << "-------------------------------------------------------"<< std::endl;
    std::cout <<"Model Name                 :" << m_CurrentNameInput->getName() << std::endl;
    std::cout <<"Controls filename          :" << m_ControlsFileName << std::endl;
    std::cout <<"State File Name            :" << m_StateFileName << std::endl;
    std::cout <<"Time Range                 :" << m_InitialTime <<" to " << m_FinalTime << "seconds!"<<std::endl;
    std::cout <<"Precision Number           :" << m_PrecisionNumber << std::endl;
    std::cout <<"Force Set File Name        :" << m_ForceSetFileName << std::endl;
    std::cout <<"External Force File Name   :" << m_ExternalForceFileName << std::endl;

    m_FDtool->setControlsFileName(m_ControlsFileName);
    m_FDtool->setStatesFileName(m_StateFileName);
    m_FDtool->setInitialTime(m_InitialTime);
    m_FDtool->setFinalTime(m_FinalTime);
    m_FDtool->setOutputPrecision(m_PrecisionNumber);
    m_FDtool->setExternalLoadsFileName(m_ExternalForceFileName);
    m_FDtool->setResultsDir(m_OutputFileName);

    m_FDtool->run();

    std::string outputfile = m_FDtool->getResultsDir();
    outputfile.append("/");
    outputfile.append(m_FDtool->getName());
    outputfile.append("_states.sto");

    vsOpenSimTools::tools->log("Running the FDTool ...");
    vsOpenSimTools::tools->log("Execution for the FD tool has finnished ...");

    OpenSim::Storage *storage = new OpenSim::Storage(outputfile);
    storage->setName("States");
    vsMotionsUtils::getInstance()->activeModel = m_CurrentNameInput;
    vsMotionsUtils::getInstance()->loadMotionStorage(storage,true,outputfile);

    QString LoadMotionName = m_FDtool->getStatesFileName().c_str();
    QVariant varient = QVariant::fromValue(LoadMotionName);
    ui->LoadedMotionCB->addItem(QString("States"), varient);
}

void vsAnalyzeToolUI::on_SavePB_clicked()
{
    //Open A File Dialog
        auto savingFileName = QFileDialog::getSaveFileName(nullptr,"Setting file for Analyze tool (*.xml)","","Setting file for forward dynamics tool (*.xml)");
        if(savingFileName == "") return;

    //get the relative paths to the saveFileName and/get the correct path
        QFileInfo savingFileInfo(savingFileName);
        QDir savingDir  = savingFileInfo.dir();
        auto relativeControlFile                = savingDir.relativeFilePath(QString::fromStdString(m_ControlsFileName));
        auto relativeInitialStateFile           = savingDir.relativeFilePath(QString::fromStdString(m_StateFileName));
        auto relativeResultDirectoryFile        = savingDir.relativeFilePath(QString::fromStdString(m_OutputFileName));
        auto relativePrecision                  = savingDir.relativeFilePath(QString::number(m_PrecisionNumber));
        auto relativeAdditionalForceSetFiles    = savingDir.relativeFilePath(QString::fromStdString(m_ForceSetFileName));
        auto relativeModelFile                  = savingDir.relativeFilePath(QString::fromStdString(m_CurrentNameInput->getInputFileName()));

    //load the current tasks before saving
        m_FDtool->setInitialTime(m_InitialTime);
        m_FDtool->setFinalTime(m_FinalTime);
        m_FDtool->setControlsFileName(relativeControlFile .toStdString());
        m_FDtool->setStatesFileName(relativeInitialStateFile.toStdString());
        m_FDtool->print(savingFileName.toStdString());
}


void vsAnalyzeToolUI::on_ClosePB_clicked()
{
    this->close();
}


void vsAnalyzeToolUI::on_HelpPB_clicked()
{
    QDesktopServices::openUrl(QUrl("https://simtk-confluence.stanford.edu/display/OpenSim/Analyses"));
}









