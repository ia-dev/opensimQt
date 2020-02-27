#ifndef vsSimulationToolsWidget_H
#define vsSimulationToolsWidget_H

#include <QWidget>

namespace Ui {
class vsSimulationToolsWidget;
}

class vsSimulationToolsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit vsSimulationToolsWidget(QWidget *parent = nullptr);
    ~vsSimulationToolsWidget();

    // QWidget interface
    virtual void dragEnterEvent(QDragEnterEvent *event) override;

private:
    Ui::vsSimulationToolsWidget *ui;

};

#endif // vsSimulationToolsWidget_H
