#ifndef VSFORWARDDYNAMICSTOOLUI_H
#define VSFORWARDDYNAMICSTOOLUI_H

#include <QWidget>

#include <QButtonGroup>
#include "vsModeling/vsNavigatorNode.h"

#include "OpenSim.h"

namespace Ui {
class vsForwardDynamicsToolUI;
}

class vsForwardDynamicsToolUI : public QWidget
{
    Q_OBJECT

public:
    explicit vsForwardDynamicsToolUI(QWidget *parent = nullptr);
    ~vsForwardDynamicsToolUI();

    OpenSim::Model *CurrentModelInput() const;
    void setCurrentModelInput(OpenSim::Model *CurrentModelInput);

private slots:
    void on_ExternalLoadsSpecificationFileBrowseTB_clicked();

private slots:
    void on_HelpBTN_clicked();

private slots:
    void on_CloseBTN_clicked();

private slots:
    void on_SaveBTN_clicked();

private slots:
    void on_RunBTN_clicked();

private slots:
        void on_InitialStateTB_clicked();

        void on_ControlsBrowseTB_clicked();

        //void on_InitialTimeRnage_valueChanged(double arg1);

        //void on_FinalTimeRnage_valueChanged(double arg1);

        void on_DirectoryBrowsefile_clicked();

        //void on_DirecorySearchfile_clicked();

        void on_LoadBTN_clicked();

        void on_DirectoryTE_textChanged();

private:
    Ui::vsForwardDynamicsToolUI *ui;

    OpenSim::Model *m_CurrentModelInput;
    OpenSim::ForwardTool *m_FDtool;
    OpenSim::AbstractTool *m_ABtool;

    //variables
    std::string m_ControlsFileName;
    std::string m_InitialStateFileName;
    int m_PrecisionNumber;
    std::string m_ForceSetFileName;
    std::string m_ExternalForceFileName;
    int m_MaximumNumberOfSteps;
    double m_MaximumStepSize;
    double m_MinimumStepSize;
    double m_IntegratorErrorTolerance;
    std::string m_modelfilename;

    double m_InitialTime;
    double m_FinalTime;
    std::string m_OutputFileName;
    std::string m_OutputFolderName;
};

#endif // VSFORWARDDYNAMICSTOOLUI_H
