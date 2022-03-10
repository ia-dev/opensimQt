#ifndef VSANALYZETOOLUI_H
#define VSANALYZETOOLUI_H

#include <QWidget>

#include <QButtonGroup>
#include "vsModeling/vsNavigatorNode.h"

#include "OpenSim.h"

namespace Ui {
class vsAnalyzeToolUI;
}

class vsAnalyzeToolUI : public QWidget
{
    Q_OBJECT

public:
    explicit vsAnalyzeToolUI(QWidget *parent = nullptr);
    ~vsAnalyzeToolUI();

    OpenSim::Model *CurrentInput() const;
    void setCurrentInput(OpenSim::Model *CurrentInput);

private slots:
    void on_StatesRB_toggled(bool checked);

private slots:
    void on_ControlsCB_toggled(bool checked);

private slots:
    void on_MotionRB_toggled(bool checked);

private slots:
    void on_FilterCoordinatesCB_toggled(bool checked);

private slots:
    void on_ActiveAnalysesEditTB_clicked();
    
private slots:
    void on_FromFileBrowseTB_clicked();

private slots:
    void on_StatesBrowseTB_clicked();

private slots:
    void on_ControlsBrowseTB_clicked();

    void on_DirectoryTB_clicked();

    void on_ExternalLoadsSpecificationFileTB_clicked();

    void on_ExternalLoadsSpecificationFileEditTB_clicked();

    void on_LoadPB_clicked();

    void on_SavePB_clicked();

    void on_RunPB_clicked();

    void on_ClosePB_clicked();

    void on_HelpPB_clicked();

private:
    Ui::vsAnalyzeToolUI *ui;

    OpenSim::Model *m_CurrentNameInput;
    OpenSim::ForwardTool *m_FDtool;

    //variable
    std::string m_ControlsFileName;
    std::string m_StateFileName;
    std::string m_FromFile;
    std::string m_OutputFileName;
    std::string m_OutputFolderName;
    std::string m_ExternalForceFileName;
    std::string m_ForceSetFileName;
    std::string m_ActiveAnalysesNames;
    int m_PrecisionNumber;
    
    double m_InitialTime;
    double m_FinalTime;
    
};

#endif // VSANALYZETOOLUI_H
