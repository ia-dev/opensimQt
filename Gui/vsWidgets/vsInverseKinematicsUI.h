#ifndef VSINVERSEKINEMATICSUI_H
#define VSINVERSEKINEMATICSUI_H

#include <QWidget>

#include <OpenSim.h>

namespace Ui {
class vsInverseKinematicsUI;
}

class vsInverseKinematicsUI : public QWidget
{
    Q_OBJECT

public:
    explicit vsInverseKinematicsUI(QWidget *parent = nullptr);
    ~vsInverseKinematicsUI();

public slots:
    void onCurrentModelChanged();


private slots:
    void on_markerDataTB_clicked();

    void on_timeRangeFromSB_valueChanged(double arg1);

    void on_timeRangeToSB_valueChanged(double arg1);

    void on_coordinateDataCB_toggled(bool checked);



    void on_coordinateDataTB_clicked();


    void on_merkerDataTE_textChanged();

    void on_coordinateDataTE_textChanged();

    void on_motionFileTE_textChanged();

    void on_motionFileTB_clicked();

private:
    Ui::vsInverseKinematicsUI *ui;
    OpenSim::Model *m_currentModel;
    OpenSim::InverseKinematicsTool *m_ikTool;

};

#endif // VSINVERSEKINEMATICSUI_H
