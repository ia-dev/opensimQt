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

public:
    explicit vsSimulationToolsWidget(QWidget *parent = nullptr);
    ~vsSimulationToolsWidget();

    // QWidget interface
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    
    int currentTime() const;

    static int TimerStep;
    
    bool repeatSimulaiton() const;

public slots:
    void setCurrentTime(int currentTime);
    void onCurrentMotionChanged();
    void onTimerTimout();

    void on_horizontalSlider_sliderReleased();
    void on_horizontalSlider_valueChanged(int value);
    void on_playButton_clicked();

    void setRepeatSimulaiton(bool repeatSimulaiton);

signals:
    void currentTimeChanged(int currentTime);

    void repeatSimulaitonChanged(bool repeatSimulaiton);


private:
    Ui::vsSimulationToolsWidget *ui;
    QTimer m_simulationTimer;
    int m_currentTime;
    //the Model and motion are from the MotionsUtil
    bool m_repeatSimulaiton;
};

#endif // vsSimulationToolsWidget_H
