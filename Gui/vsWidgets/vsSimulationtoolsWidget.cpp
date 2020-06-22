#include "vsSimulationToolsWidget.h"
#include "ui_vsSimulationToolsWidget.h"
#include <QDebug>
#include <QDragEnterEvent>
#include <vsTools/vsMotionsUtils.h>

int vsSimulationToolsWidget::TimerStep = 20;

vsSimulationToolsWidget::vsSimulationToolsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsSimulationToolsWidget),
    m_currentTime(0)
{
    ui->setupUi(this);

    m_simulationTimer.setInterval(TimerStep);
    connect(&m_simulationTimer,&QTimer::timeout,this,&vsSimulationToolsWidget::onTimerTimout);

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

bool vsSimulationToolsWidget::repeatSimulaiton() const
{
    return m_repeatSimulaiton;
}

int vsSimulationToolsWidget::currentFrame() const
{
    return m_currentFrame;
}

void vsSimulationToolsWidget::setCurrentTime(int currentTime)
{
    if (m_currentTime == currentTime)
        return;
    if(!vsMotionsUtils::getInstance()->currentMotion){
        m_simulationTimer.stop();
        return;
    }

    auto motion = vsMotionsUtils::getInstance()->currentMotion->second;
    if(currentTime > motion->getLastTime()*1000){
        m_currentTime = repeatSimulaiton()?motion->getFirstTime()*1000:motion->getLastTime()*1000;
        if(!repeatSimulaiton())m_simulationTimer.stop();
    }else{
        //it is not possible for the current time to be less then the firsttime
        m_currentTime = currentTime;
    }
    ui->currentTime->setText(QString::number((double)m_currentTime/1000.0));
    ui->horizontalSlider->setValue(m_currentTime);
    //vsMotionsUtils::getInstance()->currentMotion->first-
    vsMotionsUtils::getInstance()->applyTimeToModel(
                vsMotionsUtils::getInstance()->currentMotion->first,
                vsMotionsUtils::getInstance()->currentMotion->second,
                (double)m_currentTime/1000.0);

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
    //m_currentTime = motion->getFirstTime()*1000;
    //ui->horizontalSlider->setMinimum(0);
    //ui->horizontalSlider->setMaximum(motion->getSize()-1);
    ui->horizontalSlider->setMinimum(motion->getFirstTime()*1000);
    ui->horizontalSlider->setMaximum(motion->getLastTime()*1000);
    ui->horizontalSlider->setSingleStep(TimerStep);
    //TimerStep= motion->getMinTimeStep()*1000;
    qDebug() << "TIME_STEP: " << TimerStep;
    //setCurrentFrame(0);
    setCurrentTime(motion->getFirstTime()*1000);

}

void vsSimulationToolsWidget::onTimerTimout()
{
    //advanceTime
    setCurrentTime(m_currentTime+TimerStep);
    //setCurrentFrame(m_currentFrame+1);

}

void vsSimulationToolsWidget::on_horizontalSlider_sliderReleased()
{
    if(!vsMotionsUtils::getInstance()->currentMotion) return;
    double newTime = (double)ui->horizontalSlider->value()/1000.0;
    setCurrentTime(newTime*1000);
    vsMotionsUtils::getInstance()->applyTimeToModel(
                vsMotionsUtils::getInstance()->currentMotion->first,
                vsMotionsUtils::getInstance()->currentMotion->second,
                newTime);
//    setCurrentFrame(ui->horizontalSlider->value());
}

void vsSimulationToolsWidget::on_horizontalSlider_valueChanged(int value)
{
    //setCurrentTime(value);
    //setCurrentFrame(value);
}

void vsSimulationToolsWidget::on_playButton_clicked()
{
    //m_simulationTimer.setSingleShot(false);
    m_simulationTimer.start();
}

void vsSimulationToolsWidget::setRepeatSimulaiton(bool repeatSimulaiton)
{
    if (m_repeatSimulaiton == repeatSimulaiton)
        return;

    m_repeatSimulaiton = repeatSimulaiton;
    emit repeatSimulaitonChanged(m_repeatSimulaiton);
}

void vsSimulationToolsWidget::setCurrentFrame(int currentFrame)
{
    if (m_currentFrame == currentFrame)
        return;
    if(!vsMotionsUtils::getInstance()->currentMotion){
        m_simulationTimer.stop();
        return;
    }

    auto motion = vsMotionsUtils::getInstance()->currentMotion->second;
    if(currentFrame > motion->getSize()){
        m_currentFrame = repeatSimulaiton()?0:motion->getSize()-1;
        if(!repeatSimulaiton())m_simulationTimer.stop();
    }else{
        //it is not possible for the current time to be less then the firsttime
        m_currentFrame = currentFrame;
    }
    ui->currentTime->setText(QString::number(motion->getFirstTime()+((double)m_currentFrame*(double)TimerStep)/1000.0));
    ui->horizontalSlider->setValue(m_currentFrame);
    //vsMotionsUtils::getInstance()->currentMotion->first-
    vsMotionsUtils::getInstance()->applyFrameToModel(
                vsMotionsUtils::getInstance()->currentMotion->first,
                vsMotionsUtils::getInstance()->currentMotion->second,
                m_currentFrame);

    emit currentFrameChanged(m_currentFrame);
}
