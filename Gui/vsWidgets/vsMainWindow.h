#ifndef vsMainWindow_H
#define vsMainWindow_H

#include "vsCoordinateDelegate.h"
#include "vsCoordinatesWidget.h"
#include "vsInverseKinematicsUI.h"

#include <QMainWindow>

#include <vsWidgets/vsSimulationToolsWidget.h>
#include <vsModeling/vsNavigatorModel.h>
#include <vsModeling/vsPropertyModel.h>

QT_BEGIN_NAMESPACE
namespace Ui { class vsMainWindow; }
QT_END_NAMESPACE


/**
 * The vsMainWindow class, the main entry to the OpenSimQt GUI.
 * it is the reference widget, and all the OpenSim objects live in vsMainWindow scope
 */
class vsMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * the only constructor for the vsMainWindow that will setup the interface and connect the project signals
     * to other classes.
     * @param parent the parent widget usually nullptr as this is the main Interface
     */
    vsMainWindow(QWidget *parent = nullptr);

    /**
     * list the user plugins.
     * analyse the plugins folder and create entries in the user plugins menu.
     * also it loads the libraries marked to be loaded on entry in the config.ini file
     *
     */
    void listUserPlugins();


    ~vsMainWindow();

public slots:

    /**
     * onCurrentModelUpdated.
     * clear the coordinates and other widget data and update it with the ones
     * from the new model if it is not null
     * connected to the currentModelUpdated signal in the vsOpenSimTools
     * @see vsOpenSimTools::currentModelUpdated()
     */
    void onCurrentModelUpdated();

    /**
     * clear the running operations(simulation,motion...) on the current model
     */
    void onCleanCurrentModelProcesses();

    /**
     * respense to a user plugin being triggered.
     * a slot to load the plugin with the plugin file name
     * @param pluginFileName the fileName for the selected plugin
     */
    void userPluginClicked(QString pluginFileName);

    /**
     * the slot for the triggerd open_model action
     */
    void on_actionOpen_Model_triggered();

    /**
     * the slot for the triggered reload action
     */
    void on_actionReload_triggered();

    /**
     * show the popup menu on the navigator with the actions of NavigatorNode at that point.
     * @param point the point where the mouse was right clicked
     * @see vsNavigatorNode
     */
    void customMenuRequestedNavigator(const QPoint &point);

    /**
     * handle the NavigatorNode oneClick.
     * the slot connected to the QTreeView item clicked, it calculate the associated vsNavigatorNode
     * and update the visualizer and the parameters to display the OpenSim::Object associeted with the node
     * @param modelIndex the Qt model index that corespend to the clicked item
     * @see vsNavigatorModel::nodeForIndex()
     */
    void onNavigatorClicked(const QModelIndex modelIndex);
    void onExpendIndex(const QModelIndex modelIndex); ///< show the child nodes of the item with index=modelIndex

    /**
     * handle the changes to the object selected or clicked in the visualizer
     * @param object the object that one of its vtkProps are selected
     */
    void onSelectedObjectActor(OpenSim::Object *object);

    void on_actionSave_Model_triggered(); ///< the slot for the save action being triggered. save the current model in its inputFile

    void on_actionSave_Model_As_triggered(); ///< the slot for the save_as action triggered. save the current model in a file specified in the QFileDialog

    void on_actionSave_All_triggered(); ///< the slot for save_all action triggerd. save each open models to it input files


    void on_actionE_xit_triggered(); ///< the slot for for the exit action triggered. perform checks and confirmations and then close the application

    void on_actionClose_Model_triggered(); ///< the slot for the closeModel signal. close the current model marked as 'active'

    void on_actionClose_All_triggered(); ///< the slot for the close_all signal. close all the opened models

    void on_actionLoad_Motion_triggered();///< the slot for the load_motion action. select a motion file using a dialog and apply it to the current model if possible

    void on_posesButton_clicked();///< slot for poses_clicked signal. show the popup menu with the actions on poses @see vsDeletePoseDialog  @see vsCoordinateWidget @see vsCoordinateDelegate

private:
    Ui::vsMainWindow *ui; ///< the object containing the interface in vsMainWindow.ui compiled to c++
    vsSimulationToolsWidget *simulationWidget;///< the widget containing actions on the simulation and motion process added to the toolbar on runtime
    vsNavigatorModel *navigatorModel;///< the model used by the ui->vsNavigatorTree, containing all of the nodes Data
    vsPropertyModel *propertiesModel;///< the model for displaying the properties for the selected openSim object @see onNavigatorClicked()

    vsInverseKinematicsUI *m_IKUI;

    //Coordinates variables
    QList<vsCoordinateDelegate *> currentCoordinatesDelegates; ///< the list containing the active model coordinates delegate for the coordinate widget @see vsCoordinateWidget
    vsCoordinatesWidget *coordinatesWidget;///< the widget containing the different coordinates for the model represented using the delegate @see vsCoordinateDelegate

    //plugins
    QMap<QString,QAction*> pluginActions; ///< the map of plugins actions by name. it will load the corespending plugin to openSim when triggered

    // QWidget interface
protected:
    virtual void dropEvent(QDropEvent *event) override; ///< overrided drop event to load the droped file depending on it's extension
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dragMoveEvent(QDragMoveEvent *event) override;

    virtual void closeEvent(QCloseEvent *event) override;///<overrided close event to show the confirmation dialog
    virtual void showEvent(QShowEvent *event) override;///< update the vtkButtons position when the window is shown
    virtual void resizeEvent(QResizeEvent *event) override;///< update the vtkButtons position when the window is resized
private slots:
    void on_actionNew_Model_triggered();///< load the new model template on new_model action triggered
    void on_actionCurrent_model_Externally_triggered();///< open the current model file using an external tool, on model_externally signal triggered
    void on_actionimport_new_plugin_triggered();///< locate and add a new plugin to user plugins list on new_plugin action triggered
    void on_actionInverse_Kinematics_triggered();
};
#endif // vsMainWindow_H
