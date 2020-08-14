#ifndef vsMainWindow_H
#define vsMainWindow_H

#include "vsCoordinateDelegate.h"
#include "vsCoordinatesWidget.h"

#include <QMainWindow>

#include <vsWidgets/vsSimulationToolsWidget.h>
#include <vsModeling/vsNavigatorModel.h>
#include <vsModeling/vsPropertyModel.h>
#include <vsPython/vsPythonQt.h>

QT_BEGIN_NAMESPACE
namespace Ui { class vsMainWindow; }
QT_END_NAMESPACE

class vsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    vsMainWindow(QWidget *parent = nullptr);
    void listUserPlugins();


    ~vsMainWindow();

private slots:

    void onCurrentModelUpdated();
    void onCleanCurrentModelProcesses();

    void userPluginClicked(QString pluginFileName);

    void on_actionOpen_Model_triggered();
    void on_actionReload_triggered();

    void customMenuRequestedNavigator(const QPoint &point);
    void onNavigatorClicked(const QModelIndex modelIndex);
    void onExpendIndex(const QModelIndex modelIndex);
    void onSelectedObjectActor(OpenSim::Object *object);

    void on_actionSave_Model_triggered();

    void on_actionSave_Model_As_triggered();

    void on_actionSave_All_triggered();


    void on_actionE_xit_triggered();

    void on_actionClose_Model_triggered();

    void on_actionClose_All_triggered();

    void on_actionLoad_Motion_triggered();

    void on_posesButton_clicked();

    //! Slot: Handles the notification when history is updated on the python scripting console
    //!
    /**
     * Connect this slot to @code historyUpdate(const QString) @code signal emitted by
     * vsPythonCustomConsole object to get notified about updated history of commands executed on the console.
     *
     * @param [out] history : const QString.
     */
    void getHistory(const QString history);

private:
    Ui::vsMainWindow *ui;
    vsSimulationToolsWidget *simulationWidget;
    vsNavigatorModel *navigatorModel;
    vsPropertyModel *propertiesModel;

    //Coordinates variables
    QList<vsCoordinateDelegate *> currentCoordinatesDelegates;
    vsCoordinatesWidget *coordinatesWidget;

    //plugins
    QMap<QString,QAction*> pluginActions;

    // QWidget interface

    vsPythonQt *pythonConsole;                                              //!< Embedded Python Scripting Console :  Qt Widget for embedded python scripting
protected:
    virtual void dropEvent(QDropEvent *event) override;
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dragMoveEvent(QDragMoveEvent *event) override;

    virtual void closeEvent(QCloseEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
private slots:
    void on_actionNew_Model_triggered();

private slots:
    void on_actionCurrent_model_Externally_triggered();
    void on_actionimport_new_plugin_triggered();
    void on_actionRun_triggered();
};
#endif // vsMainWindow_H
