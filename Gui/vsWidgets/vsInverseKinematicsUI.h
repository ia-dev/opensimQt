#ifndef VSINVERSEKINEMATICSUI_H
#define VSINVERSEKINEMATICSUI_H

#include <QWidget>

#include "OpenSim.h"

namespace Ui {
class vsInverseKinematicsUI;
}

class vsInverseKinematicsUI : public QWidget
{
    Q_OBJECT

public:
    explicit vsInverseKinematicsUI(QWidget *parent = nullptr);
    ~vsInverseKinematicsUI();

    OpenSim::Model *currentModel() const;
    void setCurrentModel(OpenSim::Model *currentModel);

private:
    Ui::vsInverseKinematicsUI *ui;

    OpenSim::Model *m_currentModel;

};

#endif // VSINVERSEKINEMATICSUI_H
