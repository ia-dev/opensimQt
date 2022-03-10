/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Venkata Guru Satvik Dondapati            *

 *  Inverse Dynamics->externalForces->CreateEditExternalForces.Cppfile.    *
 *



***************************************************************************/
#include "vsCreateEditExternalForcesUI.h"
#include "ui_vsCreateEditExternalForcesUI.h"
#include "string.h"
#include <QFileDialog>
#include "vsExterenalForcesUI.h"

#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QFile>
#include <QtCore>

#include "ui_vsInverseDynamicsUI.h"
#include <vsTools/vsOpenSimTools.h>

using namespace std;

extern int i=0;

vsCreateEditExternalForcesUI::vsCreateEditExternalForcesUI(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::vsCreateEditExternalForcesUI),
    m_efTool (new OpenSim::ExternalForce()),
    m_appliedbodyname(""),
    m_forceextressedbodyname(""),
    m_pointexpressedbodyname(""),
    m_forceidentifier(""),
    m_pointidentifier(""),
    m_recordvalues(""),
    m_datasource(""),
    m_torqueidentifier("")
{
    ui->setupUi(this);


    string name = "External Force_" + to_string(i);
    int n = name.length();
    char* ptr_arr =  new char[n+1];
    strcpy(ptr_arr,name.c_str());
    ui->ForceNameTE->setPlainText(ptr_arr);
    i++;
    delete[] ptr_arr;
}

vsCreateEditExternalForcesUI::~vsCreateEditExternalForcesUI()
{
    delete ui;
}

void vsCreateEditExternalForcesUI::on_Cancel_clicked()
{
    this->close();
}


void vsCreateEditExternalForcesUI::on_Ok_clicked()
{

}


void vsCreateEditExternalForcesUI::on_AppliedToBodyCB_currentIndexChanged(const QString &arg1)
{   

}

