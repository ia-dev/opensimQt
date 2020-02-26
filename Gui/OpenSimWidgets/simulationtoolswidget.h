#ifndef SIMULATIONTOOLSWIDGET_H
#define SIMULATIONTOOLSWIDGET_H

#include <QWidget>

namespace Ui {
class SimulationToolsWidget;
}

class SimulationToolsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SimulationToolsWidget(QWidget *parent = nullptr);
    ~SimulationToolsWidget();

    // QWidget interface
    virtual void dragEnterEvent(QDragEnterEvent *event) override;

private:
    Ui::SimulationToolsWidget *ui;

};

#endif // SIMULATIONTOOLSWIDGET_H
