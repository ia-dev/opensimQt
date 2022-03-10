/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Venkata Guru Satvik Dondapati                            *

 *  Inverse Dynamics->ExternalLoads.headerfile                                                                      *
 ***************************************************************************/
#ifndef VSEXTERENALFORCESUI_H
#define VSEXTERENALFORCESUI_H

#include <QDialog>

#include "OpenSim.h"
#include <vsWidgets/vsSimulationToolsWidget.h>

#include "vsInverseDynamicsUI.h"

namespace Ui {
class vsExterenalForcesUI;
}

class vsExterenalForcesUI : public QDialog
{
    Q_OBJECT 

public:
    explicit vsExterenalForcesUI(QWidget *parent = nullptr , QString strfilename="");
    ~vsExterenalForcesUI();

    void setfilename(QString filename);

    OpenSim::Model *external() const;
    void externalloadTaskSet(OpenSim::IKTaskSet &taskSet);
    void setexternal(OpenSim::Model *external);

//public:
    OpenSim::ExternalLoads *getIDTool() const;
    void setIDTool(OpenSim::ExternalLoads *idTool);

    OpenSim::Model *externalforce() const;
    void setexternalforce(OpenSim::Model *externalforce);

    OpenSim::ExternalLoads *externalforcemodel() const;
    void setexternalforcemodel(OpenSim::ExternalLoads *externalforcemodel);

    void setDataFileName(QString dataFileName);

private slots:
    void on_Save_clicked();

private slots:
    void on_Cancel_clicked();

private slots:
    void on_Add_clicked();

private slots:
    void on_ForceDataFileTB_clicked();

private:
    Ui::vsExterenalForcesUI *ui;
    QString m_forcedatafilename;

    OpenSim::Model *m_externalspecifiedload;
    OpenSim::InverseDynamicsTool *m_idTool;
    std::string m_browsefile;
    std::string m_ExternalSpecifiedLoadText;

    std::string m_datafilename;
    OpenSim::InverseKinematicsTool *m_ikTool;
    OpenSim::ExternalLoads *m_elTool;
    OpenSim::ExternalForce *m_efTool;

};

#endif // VSEXTERENALFORCESUI_H
