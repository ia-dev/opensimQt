#include "vsInverseKinematicsUI.h"
#include "ui_vsInverseKinematicsUI.h"

#include <vsTools/vsOpenSimTools.h>

#include <vsModeling/vsNavigatorModel.h>

#include <QDebug>

vsInverseKinematicsUI::vsInverseKinematicsUI(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsInverseKinematicsUI)
{
    ui->setupUi(this);


    connect(vsOpenSimTools::tools,&vsOpenSimTools::currentModelUpdated,this,&vsInverseKinematicsUI::onCurrentModelChanged);

}

vsInverseKinematicsUI::~vsInverseKinematicsUI()
{
    delete ui;
}

void vsInverseKinematicsUI::onCurrentModelChanged()
{
    auto currentModel = vsOpenSimTools::tools->getNavigatorModel()->getActiveModel();

    if(currentModel){
        qDebug() << "the current model exist";
        auto modelName = QString::fromStdString(currentModel->getName());
        auto sizeOfMarkersSet = currentModel->updMarkerSet().getSize();

        ui->currentModelTE->setPlainText(modelName);
        ui->markerSetTE->setPlainText(QString::number(sizeOfMarkersSet)+" markers");

    }else{
        ui->currentModelTE->setPlainText("No Model is available");
        ui->markerSetTE->setPlainText("0 markers");
    }

}
