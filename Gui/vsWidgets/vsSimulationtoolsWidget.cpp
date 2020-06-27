#include "vsSimulationToolsWidget.h"
#include "ui_vsSimulationToolsWidget.h"
#include <QDebug>
#include <QDragEnterEvent>
#include <vsTools/vsMotionsUtils.h>

//Q_DECLARE_METATYPE(OpenSim::Manager::IntegratorMethod)

int vsSimulationToolsWidget::TimerStep = 33;

vsSimulationToolsWidget::vsSimulationToolsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vsSimulationToolsWidget),
    m_currentTime(0),m_repeatSimulaiton(true),m_bPlayInReverse(false),m_speedFactor(1.0)
{
    ui->setupUi(this);

    m_simulationTimer.setInterval(TimerStep);
    connect(&m_simulationTimer,&QTimer::timeout,this,&vsSimulationToolsWidget::onTimerTimout);

    //setting up integrator options
    QStringList integratorModel ;
    integratorModel << "Explicit Euler" << "Runge Kutta2" << "Runge Kutta3"
                    << "Runge Kutta Feldberg" <<"Runge Kutta Merson" << "Semi Explicit Euler2 "
                    << "Verlet";

    ui->integratorComboBox->addItems(integratorModel);
    ui->integratorComboBox->setCurrentIndex(4);

    ui->accuracySpinBox->setValue(0.0000001);
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

bool vsSimulationToolsWidget::bPlayInReverse() const
{
    return m_bPlayInReverse;
}

double vsSimulationToolsWidget::speedFactor() const
{
    return m_speedFactor;
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
    }
    else if(currentTime < motion->getFirstTime()*1000){
        m_currentTime = repeatSimulaiton()?motion->getLastTime()*1000:motion->getFirstTime()*1000;
        if(!repeatSimulaiton())m_simulationTimer.stop();
    }
    else{
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
    ui->motionGroupBox->setEnabled(true);
    ui->speedSpinBox->setEnabled(true);
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
    setCurrentTime(m_currentTime+m_speedFactor*(double)(m_bPlayInReverse?-TimerStep:TimerStep));
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
    setBPlayInReverse(false);
    m_simulationTimer.start();
}

void vsSimulationToolsWidget::on_stopToolButton_clicked()
{
    m_simulationTimer.stop();
}

void vsSimulationToolsWidget::on_playBackwordToolButton_clicked()
{
    setBPlayInReverse(true);
    m_simulationTimer.start();
}

void vsSimulationToolsWidget::on_loopToolButton_toggled(bool checked)
{
    setRepeatSimulaiton(checked);
}

void vsSimulationToolsWidget::on_nextFrameToolButton_clicked()
{
    if(!vsMotionsUtils::getInstance()->currentMotion) return;
    auto currentMotion = vsMotionsUtils::getInstance()->currentMotion->second;
    setCurrentTime(m_currentTime+currentMotion->getMinTimeStep()*1000);
//    int startFrame = 0;
//    int endFrame = 0;
//    //currentMotion->findFrameRange(((double)m_currentTime/1000.0),((double)m_currentTime/1000.0)+currentMotion->getMinTimeStep()
//    //                              startFrame,endFrame);
//    int currentFrameIndex =  currentMotion->findIndex((double)m_currentTime/1000.0);
//    //OpenSim::Array<double> timeColumn;
//    //currentMotion->getTimeColumn(timeColumn);
//    double nextFrameTime = 0;
//    currentMotion->getTime(currentFrameIndex+1,nextFrameTime);
//    qDebug() << "current frameIndex " << currentFrameIndex << "nextFrameTime " <<nextFrameTime;
//    setCurrentTime(nextFrameTime*1000);
//    //currentMotion->findFrameRange(,(double)m_currentTime/1000.0+)
    //setCurrentTime(vsMotionsUtils::getInstance()->currentMotion->second->getLastTime()*1000);

}

void vsSimulationToolsWidget::on_toEndToolButton_clicked()
{
    if(!vsMotionsUtils::getInstance()->currentMotion) return;
    setCurrentTime(vsMotionsUtils::getInstance()->currentMotion->second->getLastTime()*1000);
}

void vsSimulationToolsWidget::on_previousFrameToolButton_clicked()
{
    if(!vsMotionsUtils::getInstance()->currentMotion) return;
    auto currentMotion = vsMotionsUtils::getInstance()->currentMotion->second;
    setCurrentTime(m_currentTime-currentMotion->getMinTimeStep()*1000);
}

void vsSimulationToolsWidget::on_restartToolButton_clicked()
{
    if(!vsMotionsUtils::getInstance()->currentMotion) return;
    setCurrentTime(vsMotionsUtils::getInstance()->currentMotion->second->getFirstTime()*1000);
}

void vsSimulationToolsWidget::on_speedSpinBox_valueChanged(double arg1)
{
    setSpeedFactor(arg1);
}

void vsSimulationToolsWidget::on_spinBox_valueChanged(int arg1)
{
    TimerStep = 1000/arg1;
    m_simulationTimer.setInterval(TimerStep);
}

void vsSimulationToolsWidget::on_runSimulaitonButton_clicked()
{
    //run the simulation
    vsMotionsUtils::getInstance()->applySimulationToCurrentModel(ui->endTimeSpinBox->value());
    on_playButton_clicked();


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

void vsSimulationToolsWidget::setBPlayInReverse(bool bPlayInReverse)
{
    if (m_bPlayInReverse == bPlayInReverse)
        return;

    m_bPlayInReverse = bPlayInReverse;
    emit bPlayInReverseChanged(m_bPlayInReverse);
}

void vsSimulationToolsWidget::setSpeedFactor(double speedFactor)
{
    qWarning("Floating point comparison needs context sanity check");
    if (qFuzzyCompare(m_speedFactor, speedFactor))
        return;

    m_speedFactor = speedFactor;
    emit speedFactorChanged(m_speedFactor);
}
