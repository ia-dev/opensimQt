#ifndef vsSimulationToolsWidget_H
#define vsSimulationToolsWidget_H

#include <QWidget>

namespace Ui {
class vsSimulationToolsWidget;
}

class vsSimulationToolsWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int currentTime READ currentTime WRITE setCurrentTime NOTIFY currentTimeChanged)

public:
    explicit vsSimulationToolsWidget(QWidget *parent = nullptr);
    ~vsSimulationToolsWidget();

    // QWidget interface
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    
    int currentTime() const;
    
public slots:
    void setCurrentTime(int currentTime);
    void onCurrentMotionChanged();

    void on_horizontalSlider_sliderReleased();
    void on_horizontalSlider_valueChanged(int value);

signals:
    void currentTimeChanged(int currentTime);

private:
    Ui::vsSimulationToolsWidget *ui;
    
    int m_currentTime;
    //the Model and motion are from the MotionsUtil
};

#endif // vsSimulationToolsWidget_H
