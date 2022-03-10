/***************************************************************************
 *   Inverse Dynamics.Cpp File                                           *
 *   Copyright (C) 2020 by VisSim
 *   Authors: Venkata Guru Satvik Dondapati, Ibraheem Aldhamari          *
 *   Libraries used: m_idTool:from inversedynamicstools.h
 *                   m_eftool:from ExternalLoads.h                       *

//ID variables
     m_FromFile                 ->Browse mot file;
     m_idStartTime              -> Start for the inverse dynamics analysis.
     m_idEndTime                -> End time for the inverse dynamics analysis.
     m_FilterCoordinatesNumber  ->specifies the time historiesof kinematics that describe the movement of the model;
     m_OutputFile               ->Browse for and specify a directory in which to save the output files.
     m_OutputFolderName         ->To open an Explorer window to the specified directory.
     m_ExternalFromFile         ->External loads force specification file is an XML file that contains an ExternalLoads object.;
***************************************************************************/
#include "vsInverseDynamicsUI.h"
#include "ui_vsInverseDynamicsUI.h"
#include "vsExterenalForcesUI.h"
#include "vsCreateEditExternalForcesUI.h"
#include "string.h"

#include "vsInverseKinematicsUI.h"
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

using namespace std;

vsInverseDynamicsUI::vsInverseDynamicsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsInverseDynamicsUI),
    m_input(nullptr),
    m_idTool(new OpenSim::InverseDynamicsTool()),
    m_efTool(new OpenSim::ExternalLoads()),
    m_FromFile(""),
    m_FilterCoordinatesNumber(),
    m_ikTool(new OpenSim::InverseKinematicsTool()),
    m_idStartTime(),
    m_idEndTime(0),
    m_OutputFile(""),
    m_OutputFolderName(""),
    m_ExternalFromFile("")

{
    ui->setupUi(this);
    ui->FromFileRB->setChecked(true);
}

vsInverseDynamicsUI::~vsInverseDynamicsUI()
{
    delete ui;
}

OpenSim::Model *vsInverseDynamicsUI::input() const
{
    return m_input;
}

void vsInverseDynamicsUI::setinput(OpenSim::Model *input)
{
    m_input = input;
    if(!m_input) return;

    ui->runBTN->setEnabled(true);
    ui->FromFileRB->setChecked(true);

    m_idTool->setModel(*m_input);

    ui->LoadMotionCb->clear();
    if(m_input !=NULL)
    {
     vsNavigatorNode *modelNode = vsOpenSimTools::tools->getObjectNode(m_input);
     if(modelNode == NULL)
         return;
     vsNavigatorNode *motionsNode = modelNode->findChildNode("Motions");
     if(motionsNode == NULL)
         return;
     QList<vsNavigatorNode*> childNodes = motionsNode->childNodes;

     for(int i=0; i < childNodes.size(); i++)
     {
          QString LoadMotionName = childNodes[i]->FilePath;
          std::cout << "LoadMotionName"<< LoadMotionName.toStdString()<< std::endl;

          QVariant varient = QVariant::fromValue(LoadMotionName);
          ui->LoadMotionCb->addItem(QString(childNodes[i]->displayName), varient);
     }
     }
}

void vsInverseDynamicsUI::on_FromFileRB_toggled(bool checked)
{
    ui->FromFileTB->setEnabled(checked);
    ui->FromFileTE->setEnabled(checked);
}

void vsInverseDynamicsUI::on_FromFileTB_clicked()
{
    auto fromfile = QFileDialog::getOpenFileName(nullptr,"Motion data for Inverse Dynamics(.sto,.mot)","","Motion data for Inverse Dynamics(*.mot *.sto)");
    if(fromfile == "")return;

    m_FromFile= fromfile.toStdString();
    ui->FromFileTE->setPlainText(fromfile);
}


void vsInverseDynamicsUI::on_FromFileTE_textChanged()
{
    auto FromFileTExt  =  ui->FromFileTE->toPlainText();
    m_FromFile = FromFileTExt.toStdString();
    QFileInfo fileInfo(QString::fromStdString(m_FromFile));
    if(m_FromFile != "" && fileInfo.exists())
    {
        // that should load the ID tasks to the ID tool
        m_idTool->setCoordinatesFileName(m_FromFile);
    }

}

void vsInverseDynamicsUI::on_FilterCoordinatesCB_toggled(bool checked)
{

    ui->FilterCoordinatesTE->setEnabled(false);
    ui->FilterCoordinatesTE->setPlainText("-1");

    if(checked)
    {
        ui->FilterCoordinatesTE->setEnabled(true);
        ui->FilterCoordinatesTE->setPlainText("6");
    }

}

void vsInverseDynamicsUI::on_FilterCoordinatesTE_textChanged()
{
    // check that the  value is valid
    double m_FilterCoordinatesNumber = ui->FilterCoordinatesTE->toPlainText().toDouble();
    QFileInfo fileInfo(QString::fromStdString((to_string(m_FilterCoordinatesNumber))));
    if(to_string(m_FilterCoordinatesNumber) != "" && fileInfo.exists())
    {
        //that should load the ID tasks to the ID tool
        m_idTool->setLowpassCutoffFrequency(m_FilterCoordinatesNumber);
    }
}


void vsInverseDynamicsUI::on_LoadMotionRB_toggled(bool checked)
{
    ui->LoadMotionCb->setEnabled(checked);
}


void vsInverseDynamicsUI::on_TimeRangeFromDSB_valueChanged(double arg1)
{
     m_idStartTime =arg1;
}


void vsInverseDynamicsUI::on_TimeRangeToDSB_valueChanged(double arg1)
{
    m_idEndTime = arg1;
}

void vsInverseDynamicsUI::on_DirectoryOpenfileTB_clicked()
{
    auto outputfilename = QFileDialog::getExistingDirectory(nullptr,"Motion data for Inverse Dynamics(.mot,.sto)");
    if(outputfilename == "") return;
    m_OutputFile = outputfilename.toStdString();
    ui->DirectoryTE->setPlainText(outputfilename);
}

void vsInverseDynamicsUI::on_DirectoryTE_textChanged()
{
    m_OutputFile = ui->DirectoryTE->toPlainText().toStdString();
}


void vsInverseDynamicsUI::on_ExternalloadsspecificationfileCB_toggled(bool checked)
{
    ui->ExternalLoadsSpecificationFileTE->setEnabled(checked);
    ui->ExternalLoadSpecificationBrowseFileTB->setEnabled(checked);
    ui->externalforceTB->setEnabled(checked);
}


void vsInverseDynamicsUI::on_ExternalLoadSpecificationBrowseFileTB_clicked()
{
    auto externalfromfile = QFileDialog::getOpenFileName(nullptr,"External forces","","External forces(*.xml)");
    if(externalfromfile == "")return;
    m_ExternalFromFile= externalfromfile.toStdString();
    ui->ExternalLoadsSpecificationFileTE->setPlainText(externalfromfile);
}

void vsInverseDynamicsUI::on_runBTN_clicked()
{

    if(ui->LoadMotionRB->isChecked() == true)
    {
        int index = ui->LoadMotionCb->currentIndex();
        QVariant variant = ui->LoadMotionCb->itemData(index);
        QString strfileName = variant.toString();
        std::cout << "LodFilename"<< strfileName.toLatin1().data()<< std::endl;
        m_FromFile = strfileName.toStdString();
    }
        std::string ExternalFromFile = m_idTool->getExternalLoadsFileName();

        //check the output value is set
        if(m_OutputFile=="")
        {
            vsOpenSimTools::tools->log("No output file was specified","vsInverseDynamicsUI",vsOpenSimTools::Error);
            return;
        }
        vsOpenSimTools::tools->log("Running the IDTool ...");
        std::cout << "-------------------------------------------------------"<< std::endl;
        std::cout << "    Inverse Dynamic Tools has been started ......... "<< std::endl;
        std::cout << "-------------------------------------------------------"<< std::endl;
        std::cout <<"coordinatefilename :" << m_idTool->getCoordinatesFileName() << std::endl;
        std::cout <<"External load filename : " << ExternalFromFile << std::endl;
        std::cout <<"Model Name :             " << m_input->getName() << std::endl;
        std::cout <<"Motion file _ik.mot :    " << m_FromFile << std::endl;
        std::cout <<"Filter coordinates  :    " << m_FilterCoordinatesNumber << std::endl;
        std::cout <<"Time Range:              " << m_idStartTime <<" to " << m_idEndTime << "seconds!"<<std::endl;
        std::cout << "ID Tool info .............. "<< std::endl;
        std::cout <<"coordinatefile Name: " << m_idTool->getCoordinatesFileName() << std::endl;
        std::cout <<"Filter Coordinates: " << m_idTool->getLowpassCutoffFrequency() << std::endl;
        std::cout <<"External Loads Specificarion File: " << m_idTool->getExternalLoadsFileName() << std::endl;
        std::cout <<"Output Result Motion File :" << m_OutputFile << std::endl;
        std::cout <<"Outputgenforcefilename :" << m_idTool->getOutputGenForceFileName() << std::endl;

        m_idTool->setStartTime(m_idStartTime);
        m_idTool->setEndTime(m_idEndTime);
        m_idTool->setLowpassCutoffFrequency(m_FilterCoordinatesNumber);
        if(ui->FilterCoordinatesCB->isChecked()) m_idTool->setLowpassCutoffFrequency(m_FilterCoordinatesNumber);
        m_idTool->setResultsDir(m_OutputFile);
        if(ui->ExternalloadsspecificationfileCB->isChecked()) m_idTool->setExternalLoadsFileName( (ExternalFromFile));
        vsOpenSimTools::tools->log("Running the IdTool ...");

        m_idTool->run();
        vsOpenSimTools::tools->log("Execution for the ID tool has finnished ...");
        string outputfile = m_OutputFile;
        outputfile.append("\\inverse_dynamics.sto");

        OpenSim::Storage *storage = new OpenSim::Storage(outputfile);
        storage->setName("ID Results");
        vsMotionsUtils::getInstance()->activeModel = m_input;
        vsMotionsUtils::getInstance()->loadMotionStorage(storage,true, outputfile);

        QString LoadMotionName = outputfile.c_str();
        QVariant varient = QVariant::fromValue(LoadMotionName);
        ui->LoadMotionCb->addItem(QString("ID Rsesult"), varient);

}

void vsInverseDynamicsUI::on_externalforceTB_clicked()
{
    if(!m_ExternalFromFile.empty())
    {
        std::cout <<"externalfromfile 1:" << m_ExternalFromFile << std::endl;
        vsExterenalForcesUI* externalforce = new vsExterenalForcesUI(this, m_ExternalFromFile.c_str());
        std::string txt = ui->ExternalLoadsSpecificationFileTE->toPlainText().toStdString();
        std::cout <<"externalfromfile 2:" << m_ExternalFromFile << std::endl;
        externalforce->setModal(true);
        externalforce->exec();

    } else{
            vsExterenalForcesUI* externalforce = new vsExterenalForcesUI(this);
            std::cout <<"externalfromfile:" << m_ExternalFromFile << std::endl;
            externalforce->setModal(true);
            externalforce->exec();
        }
}


void vsInverseDynamicsUI::on_Load_clicked()
{
    m_idTool->setModel(*m_input);
    //select the configuration file
    auto configFileName =  QFileDialog::getOpenFileName(nullptr,"ID tool setting file","","ID tool setting file (*.xml)");
    if(configFileName == "") return;

    ui->FilterCoordinatesCB->setChecked(true);
    ui->FromFileRB->setChecked(true);
    ui->ExternalloadsspecificationfileCB->setChecked(true);

    //create a new instance of the Id tool
    m_idTool = new OpenSim::InverseDynamicsTool(configFileName.toStdString(),false);

    qDebug() << "Coordinate file name form the config file :" << QString::fromStdString(m_idTool->getCoordinatesFileName());

    //convert the relative path to an absolute one (for the registered files)
    auto configFileDir = QFileInfo(configFileName).dir();
    m_FromFile = configFileDir.cleanPath(configFileDir.absoluteFilePath(QString::fromStdString(m_idTool->getCoordinatesFileName()))).toStdString();  
    m_OutputFile = configFileDir.cleanPath(configFileDir.absoluteFilePath(QString::fromStdString(m_idTool->getResultsDir()))).toStdString();
    m_ExternalFromFile=configFileDir.cleanPath(configFileDir.absoluteFilePath(QString::fromStdString(m_idTool->getExternalLoadsFileName()))).toStdString();
    m_FilterCoordinatesNumber =m_idTool->getLowpassCutoffFrequency();

    std::cout <<"getCoordinatesFilename:" << m_idTool->getCoordinatesFileName() << std::endl;
    std::cout <<"external force file111:" << m_ExternalFromFile << std::endl;
    std::cout <<"outputmotionfile" << m_idTool->getOutputGenForceFileName() << std::endl;
    qDebug() << "external force file:" << QString::fromStdString(m_idTool->getExternalLoadsFileName());

    //update the UI
    ui->FilterCoordinatesTE->setPlainText(QString::number(m_FilterCoordinatesNumber));
    ui->FromFileTE->setPlainText(QString::fromStdString(m_FromFile));
    ui->DirectoryTE->setPlainText(QString::fromStdString(m_OutputFile));
    ui->TimeRangeFromDSB->setValue(m_idTool->getStartTime());
    ui->TimeRangeToDSB->setValue(m_idTool->getEndTime());
    ui->ExternalLoadsSpecificationFileTE->setPlainText(QString::fromStdString(m_ExternalFromFile));
}

void vsInverseDynamicsUI::on_SaveBTN_clicked()
{
//Open A File Dialog
    auto savingFileName = QFileDialog::getSaveFileName(nullptr,"IK tool setting file","","IK tool setting file (*.xml)");
    if(savingFileName == "") return;

//get the relative paths to the saveFileName and/get the correct path
    QFileInfo savingFileInfo(savingFileName);
    QDir savingDir  = savingFileInfo.dir();
    auto relativeFromfile = savingDir.relativeFilePath(QString::fromStdString(m_FromFile));
    auto relativeCoordinatenumber = savingDir.relativeFilePath(QString::number(m_FilterCoordinatesNumber));
    auto relativeOutputFile = savingDir.relativeFilePath(QString::fromStdString(m_OutputFile));
    auto relativeModelFile = savingDir.relativeFilePath(QString::fromStdString(m_input->getInputFileName()));

//load the current tasks before saving
    m_idTool->setStartTime(m_idStartTime);
    m_idTool->setEndTime(m_idEndTime);
    m_idTool->setModelFileName(relativeModelFile .toStdString());
    m_idTool->setOutputGenForceFileName(relativeOutputFile.toStdString());
    m_idTool->print(savingFileName.toStdString());
}

void vsInverseDynamicsUI::on_CloseBTN_clicked()
{
    this->close();
}

void vsInverseDynamicsUI::on_HelpBTN_clicked()
{
    QDesktopServices::openUrl(QUrl("https://simtk-confluence.stanford.edu/display/OpenSim40/Inverse+Dynamics"));
}
