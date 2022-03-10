/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Venkata Guru Satvik Dondapati                            *

 *  Inverse Dynamics->externalForces->CreateEditExternalForces.headerfile                                                                      *
 ***************************************************************************/
#ifndef VSCREATEEDITEXTERNALFORCESUI_H
#define VSCREATEEDITEXTERNALFORCESUI_H

#include <QDialog>
#include "OpenSim.h"
#include "vsInverseDynamicsUI.h"

namespace Ui {
class vsCreateEditExternalForcesUI;
}

class vsCreateEditExternalForcesUI : public QDialog
{
    Q_OBJECT

public:
    explicit vsCreateEditExternalForcesUI(QWidget *parent = nullptr);
    ~vsCreateEditExternalForcesUI();

private slots:
    void on_AppliedToBodyCB_currentIndexChanged(const QString &arg1);

private slots:
    void on_Ok_clicked();

private slots:
    void on_Cancel_clicked();

private:
    Ui::vsCreateEditExternalForcesUI *ui;

    OpenSim::ExternalForce *m_efTool;
    std::string m_appliedbodyname;
    std::string m_forceextressedbodyname;
    std::string m_pointexpressedbodyname;
    std::string m_forceidentifier;
    std::string m_pointidentifier;
    std::string m_recordvalues;
    std::string m_datasource;
    std::string m_torqueidentifier;

};

#endif // VSCREATEEDITEXTERNALFORCESUI_H
