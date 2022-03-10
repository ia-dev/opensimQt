/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Venkata Guru Satvik Dondapati                            *

 *  Inverse Dynamics->externalForces.cpp file                                                                     *
 ***************************************************************************/
#include "vsExterenalForcesUI.h"
#include "ui_vsExterenalForcesUI.h"
#include <QFileDialog>
#include "vsCreateEditExternalForcesUI.h"
#include "ui_vsInverseDynamicsUI.h"
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QFile>
#include <QtCore>


#include <vsTools/vsOpenSimTools.h>
extern int i;

vsExterenalForcesUI::vsExterenalForcesUI(QWidget *parent, QString strfilename) :
    QDialog(parent),
    ui(new Ui::vsExterenalForcesUI),
    m_elTool (new OpenSim::ExternalLoads()),
    m_efTool (new OpenSim::ExternalForce()),
    m_browsefile(""),
    m_datafilename(""),
    m_ExternalSpecifiedLoadText(""),
    m_externalspecifiedload(nullptr)

{
    ui->setupUi(this);
    m_forcedatafilename = strfilename;

    QString forcedatafilename = QString::fromStdString(m_forcedatafilename.toStdString());
    if(forcedatafilename == "")return;

    std::cout <<"Forcedatafilename:" << m_forcedatafilename.toStdString() << std::endl;

    m_elTool = new OpenSim::ExternalLoads (m_forcedatafilename.toStdString(),true);

     std::cout <<"force data name:" << m_elTool->getDataFileName() << std::endl;

    auto configFileDir = QFileInfo(m_forcedatafilename).dir();

    m_ExternalSpecifiedLoadText = configFileDir.cleanPath(configFileDir.absoluteFilePath(QString::fromStdString(m_elTool->getDataFileName()))).toStdString();

    ui->ForceDataFileTE->setPlainText(QString::fromStdString(m_ExternalSpecifiedLoadText));

}

vsExterenalForcesUI::~vsExterenalForcesUI()
{
    delete ui;
}

OpenSim::Model *vsExterenalForcesUI::external() const
{
    return m_externalspecifiedload;
}

void vsExterenalForcesUI::setDataFileName(QString dataFileName)
{
    ui->ForceDataFileTE->setPlainText(dataFileName);
}

void vsExterenalForcesUI::on_ForceDataFileTB_clicked()
{
    auto browsefile = QFileDialog::getOpenFileName(nullptr,"Data file for external forces","","Data file for external forces(*.mot) ;; (*.sto)");
    if(browsefile == "") return;

    m_browsefile= browsefile.toStdString();

    ui->ForceDataFileTE->setPlainText(browsefile);
}

void vsExterenalForcesUI::on_Add_clicked()
{
    vsCreateEditExternalForcesUI createexternalforce;
    createexternalforce.setModal(true);
    createexternalforce.exec();
}

void vsExterenalForcesUI::on_Cancel_clicked()
{
    i=0;
   this->close();

}

void vsExterenalForcesUI::on_Save_clicked()
{

}

void vsExterenalForcesUI::setfilename(QString filename){
    this->m_forcedatafilename = filename;
}
