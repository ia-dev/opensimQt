#ifndef vsSimulationToolsWidget_H
#define vsSimulationToolsWidget_H

#include <QWidget>
#include <QTimer>

namespace Ui {
class vsSimulationToolsWidget;
}

class vsSimulationToolsWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int currentTime READ currentTime WRITE setCurrentTime NOTIFY currentTimeChanged)
    Q_PROPERTY(bool repeatSimulaiton READ repeatSimulaiton WRITE setRepeatSimulaiton NOTIFY repeatSimulaitonChanged)
    Q_PROPERTY(int currentFrame READ currentFrame WRITE setCurrentFrame NOTIFY currentFrameChanged)
    Q_PROPERTY(bool bPlayInReverse READ bPlayInReverse WRITE setBPlayInReverse NOTIFY bPlayInReverseChanged)
    Q_PROPERTY(double speedFactor READ speedFactor WRITE setSpeedFactor NOTIFY speedFactorChanged)

public:
    explicit vsSimulationToolsWidget(QWidget *parent = nullptr);
    ~vsSimulationToolsWidget();

    // QWidget interface
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    
    int currentTime() const;

    static int TimerStep;
    
    bool repeatSimulaiton() const;

    int currentFrame() const;

    bool bPlayInReverse() const;

    double speedFactor() const;

public slots:
    void setCurrentTime(int currentTime);
    void onCurrentMotionChanged();
    void onTimerTimout();

    void on_horizontalSlider_sliderReleased();
    void on_horizontalSlider_valueChanged(int value);
    void on_playButton_clicked();
    void on_stopToolButton_clicked();
    void on_playBackwordToolButton_clicked();
    void on_loopToolButton_toggled(bool checked);
    void on_nextFrameToolButton_clicked();
    void on_toEndToolButton_clicked();
    void on_previousFrameToolButton_clicked();
    void on_restartToolButton_clicked();
    void on_speedSpinBox_valueChanged(double arg1);
    void on_spinBox_valueChanged(int arg1);

    void on_runSimulaitonButton_clicked();

    void setRepeatSimulaiton(bool repeatSimulaiton);

    void setCurrentFrame(int currentFrame);

    void setBPlayInReverse(bool bPlayInReverse);

    void setSpeedFactor(double speedFactor);

signals:
    void currentTimeChanged(int currentTime);

    void repeatSimulaitonChanged(bool repeatSimulaiton);

    void currentFrameChanged(int currentFrame);

    void bPlayInReverseChanged(bool bPlayInReverse);

    void speedFactorChanged(double speedFactor);


private slots:


private:
    Ui::vsSimulationToolsWidget *ui;
    QTimer m_simulationTimer;
    int m_currentTime;
    //the Model and motion are from the MotionsUtil
    bool m_repeatSimulaiton;
    int m_currentFrame;
    bool m_bPlayInReverse;
    double m_speedFactor;
};

#endif // vsSimulationToolsWidget_H
