#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <OpenSimWidgets/simulationtoolswidget.h>
#include <Modeling/navigatormodel.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_Model_triggered();

private:
    Ui::MainWindow *ui;
    SimulationToolsWidget *simulationWidget;
    NavigatorModel *navigatorModel;

    // QWidget interface
protected:
    virtual void dropEvent(QDropEvent *event) override;
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dragMoveEvent(QDragMoveEvent *event) override;
};
#endif // MAINWINDOW_H
