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

public:
    explicit vsSimulationToolsWidget(QWidget *parent = nullptr);
    ~vsSimulationToolsWidget();

    // QWidget interface
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    
    int currentTime() const;

    static int TimerStep;
    
    bool repeatSimulaiton() const;

    int currentFrame() const;

public slots:
    void setCurrentTime(int currentTime);
    void onCurrentMotionChanged();
    void onTimerTimout();

    void on_horizontalSlider_sliderReleased();
    void on_horizontalSlider_valueChanged(int value);
    void on_playButton_clicked();

    void setRepeatSimulaiton(bool repeatSimulaiton);

    void setCurrentFrame(int currentFrame);

signals:
    void currentTimeChanged(int currentTime);

    void repeatSimulaitonChanged(bool repeatSimulaiton);


    void currentFrameChanged(int currentFrame);

private:
    Ui::vsSimulationToolsWidget *ui;
    QTimer m_simulationTimer;
    int m_currentTime;
    //the Model and motion are from the MotionsUtil
    bool m_repeatSimulaiton;
    int m_currentFrame;
};

#endif // vsSimulationToolsWidget_H
