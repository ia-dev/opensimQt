#include "vsPython/vsManager.h"
#include <QDebug>


vsManager::vsManager(QObject *parent) : QObject{parent}
{
    sp.flag = sp.flag & SimParamFlag::None;
    QString classVar = "vsManager";
    //vsSimulationToolsWidget::on_runSimulaitonButton_clicked()

}

void vsManager::setAccuracy(double accuracy)
{
    sp.flag = sp.flag | SimParamFlag::Accuracy;
    sp.accuracy = accuracy;
}

void vsManager::setStepSize(double stepSize)
{
    sp.flag = sp.flag | SimParamFlag::StepSize;
    sp.stepSize = stepSize;
}

void vsManager::setEndTime(double endTime)
{
    sp.flag = sp.flag | SimParamFlag::EndTime;
    sp.endTime = endTime;
}

void vsManager::setIntegratorMethod(vsManager::IntegratorMethod integratorMethod)
{
    sp.flag = sp.flag | SimParamFlag::IntegratorMethod;
    sp.integratorMethod = static_cast<int>(integratorMethod);
}

void vsManager::updSimDataToGui()
{
    emit updateSimulationParams(sp);
}

void vsManager::runSimulation()
{

}

void vsManager::stopSimulation()
{

}

