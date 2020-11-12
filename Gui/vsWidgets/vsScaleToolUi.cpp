#include "vsScaleToolUi.h"
#include "ui_vsScaleToolUi.h"

vsScaleToolUI::vsScaleToolUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsScaleToolUI),
    m_sclaeTool(new OpenSim::ScaleTool()),
    m_currentModel(nullptr)
{
    ui->setupUi(this);
}

vsScaleToolUI::~vsScaleToolUI()
{
    delete ui;
}

OpenSim::Model *vsScaleToolUI::currentModel() const
{
    return m_currentModel;
}

void vsScaleToolUI::setCurrentModel(OpenSim::Model *currentModel)
{
    m_currentModel = currentModel;

    ui->modelNameLE->setText(QString::fromStdString(m_currentModel->getName())+"scaled");
    ui->genericModelNameLE->setText(QString::fromStdString(m_currentModel->getName()));
    auto markersSetSize = m_currentModel->updMarkerSet().getSize();

    ui->genericMarkerSetLE->setText(QString::number(markersSetSize));
    ui->resultingMarkersLE->setText(QString::number(markersSetSize));


    //auto setSize = m_currentModel->mass
    //ui->genericMassSB->setValue(markersSetSize);


}
