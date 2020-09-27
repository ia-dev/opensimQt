#include "vsInverseKinematicsUI.h"
#include "ui_vsInverseKinematicsUI.h"

vsInverseKinematicsUI::vsInverseKinematicsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsInverseKinematicsUI),
    m_currentModel(nullptr)
{
    ui->setupUi(this);
}

vsInverseKinematicsUI::~vsInverseKinematicsUI()
{
    delete ui;
}

OpenSim::Model *vsInverseKinematicsUI::currentModel() const
{
    return m_currentModel;
}

void vsInverseKinematicsUI::setCurrentModel(OpenSim::Model *currentModel)
{
    m_currentModel = currentModel;
    if(!m_currentModel) return;

    //You need to convert the string to QString instead of std::string
    ui->modelNameTE->setPlainText(QString::fromStdString(m_currentModel->getName()));
    auto markersSetSize = m_currentModel->updMarkerSet().getSize();

    ui->markersSetTE->setPlainText(QString::number(markersSetSize)+" markers");

}
