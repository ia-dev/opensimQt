#ifndef vsMainWindow_H
#define vsMainWindow_H

#include <QMainWindow>

#include <vsWidgets/vsSimulationToolsWidget.h>
#include <vsModeling/vsNavigatorModel.h>

QT_BEGIN_NAMESPACE
namespace Ui { class vsMainWindow; }
QT_END_NAMESPACE

class vsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    vsMainWindow(QWidget *parent = nullptr);
    ~vsMainWindow();

private slots:
    void on_actionOpen_Model_triggered();
    void on_actionReload_triggered();

    void customMenuRequestedNavigator(const QPoint &point);
    void onExpendIndex(const QModelIndex modelIndex);

    void on_actionSave_Model_triggered();

    void on_actionSave_Model_As_triggered();

    void on_actionSave_All_triggered();


    void on_actionE_xit_triggered();

private:
    Ui::vsMainWindow *ui;
    vsSimulationToolsWidget *simulationWidget;
    vsNavigatorModel *navigatorModel;

    // QWidget interface
protected:
    virtual void dropEvent(QDropEvent *event) override;
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dragMoveEvent(QDragMoveEvent *event) override;

    virtual void closeEvent(QCloseEvent *event) override;
};
#endif // vsMainWindow_H
