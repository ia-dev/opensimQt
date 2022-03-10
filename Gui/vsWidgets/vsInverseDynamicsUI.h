/***************************************************************************
 *                                                                         *
 *   Copyright (C) 2020 by VisSim                                          *
 *                                                                         *
 *   Authors: Ibraheem Aldhamari, Venkata Guru Satvik Dondapati                            *

 *  Inverse Dynamics.Header File                                                                       *
 ***************************************************************************/
#ifndef VSINVERSEDYNAMICSUI_H
#define VSINVERSEDYNAMICSUI_H

#include <QWidget>

#include <QButtonGroup>
#include "vsModeling/vsNavigatorNode.h"

#include "OpenSim.h"
#include "vsExterenalForcesUI.h"

namespace Ui {
class vsInverseDynamicsUI;
}

class vsInverseDynamicsUI : public QWidget
{
    Q_OBJECT

public:
    explicit vsInverseDynamicsUI(QWidget *parent = nullptr);
    ~vsInverseDynamicsUI();

    OpenSim::Model *input() const;
    void setinput(OpenSim::Model *input);

private slots:
    void on_FromFileRB_toggled(bool checked);

private slots:
    void on_HelpBTN_clicked();

private slots:
    void on_CloseBTN_clicked();

private slots:
    void on_SaveBTN_clicked();

private slots:
    void on_Load_clicked();

private slots:
    void on_externalforceTB_clicked();

private slots:
    void on_runBTN_clicked();

private slots:
    void on_ExternalLoadSpecificationBrowseFileTB_clicked();

private slots:
    void on_ExternalloadsspecificationfileCB_toggled(bool checked);

private slots:
   void on_DirectoryOpenfileTB_clicked();

private slots:
    void on_DirectoryTE_textChanged();

private slots:
    void on_TimeRangeToDSB_valueChanged(double arg1);

private slots:
    void on_TimeRangeFromDSB_valueChanged(double arg1);

private slots:
    void on_LoadMotionRB_toggled(bool checked);

private slots:
    void on_FilterCoordinatesTE_textChanged();

private slots:
    void on_FilterCoordinatesCB_toggled(bool checked);

private slots:
    void on_FromFileTE_textChanged();

private slots:
    void on_FromFileTB_clicked();

private:
    Ui::vsInverseDynamicsUI *ui;

    //using functions
    OpenSim::Model *m_input;
    OpenSim::InverseDynamicsTool *m_idTool;
    OpenSim::InverseKinematicsTool *m_ikTool;
    OpenSim::ExternalLoads *m_efTool;
    QButtonGroup valueTypeBG;


    //ID variables
    std::string m_FromFile;
    double m_idStartTime;
    double m_idEndTime;
    double m_FilterCoordinatesNumber;
    std::string m_OutputFile;
    std::string m_OutputFolderName;
    std::string m_ExternalFromFile;
};

#endif // VSINVERSEDYNAMICSUI_H
