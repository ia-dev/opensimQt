#include "vsSimulationToolsWidget.h"
#include "ui_vsSimulationToolsWidget.h"
#include <QDebug>
#include <QDragEnterEvent>
#include <vsTools/vsMotionsUtils.h>

vsSimulationToolsWidget::vsSimulationToolsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsSimulationToolsWidget),
    m_currentTime(0)
{
    ui->setupUi(this);

}

vsSimulationToolsWidget::~vsSimulationToolsWidget()
{
    delete ui;
}

void vsSimulationToolsWidget::dragEnterEvent(QDragEnterEvent *event)
{

}

int vsSimulationToolsWidget::currentTime() const
{
    return m_currentTime;
}

void vsSimulationToolsWidget::setCurrentTime(int currentTime)
{
    if (m_currentTime == currentTime)
        return;
    
    m_currentTime = currentTime;
    emit currentTimeChanged(m_currentTime);
}

void vsSimulationToolsWidget::onCurrentMotionChanged()
{
    qDebug() << "current motion changed";
    auto motion = vsMotionsUtils::getInstance()->currentMotion->second;
    //test if the motion is present
    if(!vsMotionsUtils::getInstance()->currentMotion) return;
    //update the display times/labels
    ui->motinNameEdit->setText(QString::fromStdString(vsMotionsUtils::getInstance()->currentMotion->second->getName()));
    ui->currentTime->setText(QString::number(vsMotionsUtils::getInstance()->currentMotion->second->getFirstTime()));
    ui->horizontalSlider->setMinimum(motion->getFirstTime()*100);
    ui->horizontalSlider->setMaximum(motion->getLastTime()*100);
    ui->horizontalSlider->setSingleStep(10);

}

void vsSimulationToolsWidget::on_horizontalSlider_sliderReleased()
{
    if(!vsMotionsUtils::getInstance()->currentMotion) return;
    double newTime = (double)ui->horizontalSlider->value()/100.0;
    ui->currentTime->setText(QString::number(newTime));
    vsMotionsUtils::getInstance()->applyTimeToModel(
                vsMotionsUtils::getInstance()->currentMotion->first,
                vsMotionsUtils::getInstance()->currentMotion->second,
                newTime);
}

void vsSimulationToolsWidget::on_horizontalSlider_valueChanged(int value)
{
    double newTime = (double)ui->horizontalSlider->value()/100.0;
    ui->currentTime->setText(QString::number(newTime));
}
